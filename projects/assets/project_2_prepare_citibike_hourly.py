#!/usr/bin/env python3
"""Build the course hourly Citi Bike dataset from the official monthly ZIPs.

The script uses only the Python standard library.  It reads every CSV member in
the supplied archives, finds the ``started_at`` column by name, and counts trip
starts by local New York hour in January and February 2026.
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import io
import json
import math
import os
import tempfile
import zipfile
from datetime import date, datetime, timedelta
from pathlib import Path
from zoneinfo import ZoneInfo


START = datetime(2026, 1, 1, 0, 0, 0)
END = datetime(2026, 3, 1, 0, 0, 0)
EXPECTED_HOURS = 1416
NEW_YORK = ZoneInfo("America/New_York")
HOLIDAYS = {date(2026, 1, 1), date(2026, 1, 19), date(2026, 2, 16)}
HEADER = [
    "timestamp",
    "count",
    "hour_sin",
    "hour_cos",
    "dow_sin",
    "dow_cos",
    "is_weekend",
    "day_index",
    "is_february",
    "is_holiday",
]


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def parse_started_at(raw: str, location: str) -> datetime:
    text = raw.strip()
    if not text:
        raise ValueError(f"{location}: empty started_at value")
    try:
        parsed = datetime.fromisoformat(text)
    except ValueError as error:
        raise ValueError(f"{location}: invalid started_at value {text!r}") from error

    if parsed.tzinfo is not None:
        parsed = parsed.astimezone(NEW_YORK).replace(tzinfo=None)
    return parsed


def csv_members(archive: zipfile.ZipFile) -> list[zipfile.ZipInfo]:
    members = []
    for member in archive.infolist():
        basename = Path(member.filename).name
        if member.is_dir() or not basename.lower().endswith(".csv"):
            continue
        if basename.upper().startswith("JC"):
            continue
        members.append(member)
    return sorted(members, key=lambda member: member.filename)


def count_trip_starts(archive_paths: list[Path]) -> tuple[list[int], dict[str, object]]:
    counts = [0] * EXPECTED_HOURS
    total_rows = 0
    accepted_rows = 0
    outside_interval = 0
    member_names: list[str] = []

    for archive_path in archive_paths:
        if not archive_path.is_file():
            raise FileNotFoundError(f"archive not found: {archive_path}")
        if not zipfile.is_zipfile(archive_path):
            raise ValueError(f"not a ZIP archive: {archive_path}")

        with zipfile.ZipFile(archive_path) as archive:
            members = csv_members(archive)
            if not members:
                raise ValueError(f"no non-Jersey-City CSV files in {archive_path}")

            for member in members:
                member_names.append(f"{archive_path.name}:{member.filename}")
                with archive.open(member, "r") as binary_stream:
                    with io.TextIOWrapper(
                            binary_stream, encoding="utf-8-sig", newline=""
                    ) as text_stream:
                        reader = csv.DictReader(text_stream)
                        if reader.fieldnames is None or "started_at" not in reader.fieldnames:
                            raise ValueError(
                                f"{archive_path.name}:{member.filename}: "
                                "missing started_at column"
                            )
                        for row_number, row in enumerate(reader, start=2):
                            total_rows += 1
                            location = (
                                f"{archive_path.name}:{member.filename}:"
                                f"row {row_number}"
                            )
                            started_at = parse_started_at(
                                row.get("started_at") or "", location
                            )
                            if not START <= started_at < END:
                                outside_interval += 1
                                continue
                            hour = started_at.replace(
                                minute=0, second=0, microsecond=0
                            )
                            index = int((hour - START).total_seconds() // 3600)
                            counts[index] += 1
                            accepted_rows += 1

    metadata: dict[str, object] = {
        "input_rows": total_rows,
        "accepted_rows": accepted_rows,
        "outside_interval_rows": outside_interval,
        "csv_members": member_names,
    }
    return counts, metadata


def feature_row(timestamp: datetime, count: int) -> list[str | int]:
    hour = timestamp.hour
    weekday = timestamp.weekday()
    return [
        timestamp.strftime("%Y-%m-%d %H:%M:%S"),
        count,
        format(math.sin(2.0 * math.pi * hour / 24.0), ".17g"),
        format(math.cos(2.0 * math.pi * hour / 24.0), ".17g"),
        format(math.sin(2.0 * math.pi * weekday / 7.0), ".17g"),
        format(math.cos(2.0 * math.pi * weekday / 7.0), ".17g"),
        int(weekday >= 5),
        (timestamp.date() - START.date()).days,
        int(timestamp.month == 2),
        int(timestamp.date() in HOLIDAYS),
    ]


def write_output(output_path: Path, counts: list[int]) -> None:
    if len(counts) != EXPECTED_HOURS:
        raise ValueError(
            f"expected {EXPECTED_HOURS} hourly counts, received {len(counts)}"
        )
    output_path.parent.mkdir(parents=True, exist_ok=True)
    descriptor, temporary_name = tempfile.mkstemp(
        prefix=f".{output_path.name}.", suffix=".tmp", dir=output_path.parent
    )
    try:
        with os.fdopen(descriptor, "w", encoding="utf-8", newline="") as stream:
            writer = csv.writer(stream, lineterminator="\n")
            writer.writerow(HEADER)
            for index, count in enumerate(counts):
                timestamp = START + timedelta(hours=index)
                writer.writerow(feature_row(timestamp, count))
                os.replace(temporary_name, output_path)
    except BaseException:
        try:
            os.unlink(temporary_name)
        except FileNotFoundError:
            pass
        raise


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "archives",
        nargs="+",
        type=Path,
        help="official Citi Bike monthly ZIP archives",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=Path("citibike_hourly_2026_01_02.csv"),
        help="output CSV path",
    )
    return parser.parse_args()


def main() -> None:
    arguments = parse_arguments()
    counts, metadata = count_trip_starts(arguments.archives)
    write_output(arguments.output, counts)

    summary = {
        "source": "https://citibikenyc.com/system-data",
        "data_use_policy": "https://citibikenyc.com/data-sharing-policy",
        "archives": [
            {
                "name": path.name,
                "size_bytes": path.stat().st_size,
                "sha256": sha256_file(path),
            }
            for path in arguments.archives
        ],
        **metadata,
        "output": {
            "name": arguments.output.name,
            "rows": EXPECTED_HOURS,
            "total_trips": sum(counts),
            "size_bytes": arguments.output.stat().st_size,
            "sha256": sha256_file(arguments.output),
        },
    }
    print(json.dumps(summary, indent=2))


if __name__ == "__main__":
    main()
