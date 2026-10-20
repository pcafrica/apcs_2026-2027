#include "data_io.hpp"

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

void stop_with_error(const char *message) {
  std::cerr << "Error: " << message << '\n';
  std::exit(EXIT_FAILURE);
}

namespace {
void read_numbers(std::istream &input, std::vector<double> &values) {
  values.clear();
  while (true) {
    input >> std::ws;
    if (input.eof()) {
      break;
    }
    double value;
    if (!(input >> value) || !std::isfinite(value)) {
      stop_with_error("Expected finite numbers separated by whitespace.");
    }
    values.push_back(value);
  }
  if (values.empty()) {
    stop_with_error("The dataset must not be empty.");
  }
}
} // namespace

void DataBuffer::read_file(const std::string &filename) {
  // A new local stream is opened on every call and closed automatically.
  std::ifstream input(filename);
  if (!input.is_open()) {
    stop_with_error("Cannot open the input file.");
  }
  read_numbers(input, values);
}

void DataBuffer::read_console() {
  std::cout << "Enter numbers separated by spaces on one non-empty line:\n";
  std::string line;
  // Skip pending whitespace, without discarding the first data character.
  if (!std::getline(std::cin >> std::ws, line)) {
    stop_with_error("Cannot read console data.");
  }
  std::istringstream input(line);
  read_numbers(input, values);
}

std::size_t DataBuffer::n_elements() const { return values.size(); }

double &DataBuffer::operator[](std::size_t index) {
  if (index >= values.size()) {
    stop_with_error("Data index out of range.");
  }
  return values[index];
}

const double &DataBuffer::operator[](std::size_t index) const {
  if (index >= values.size()) {
    stop_with_error("Data index out of range.");
  }
  return values[index];
}

namespace {
std::vector<std::string> split_csv_line(std::string line) {
  if (!line.empty() && line.back() == '\r') {
    line.pop_back();
  }
  std::vector<std::string> cells;
  std::istringstream input(line);
  std::string cell;
  while (std::getline(input, cell, ',')) {
    cells.push_back(cell);
  }
  if (!line.empty() && line.back() == ',') {
    cells.push_back("");
  }
  return cells;
}
} // namespace

void DataBuffer::read_csv(const std::string &filename,
                          const std::string &field) {
  std::ifstream input(filename);
  if (!input.is_open()) {
    stop_with_error("Cannot open the CSV file.");
  }
  std::string line;
  if (!std::getline(input, line)) {
    stop_with_error("Missing CSV header.");
  }
  const std::vector<std::string> header = split_csv_line(line);
  std::size_t column = header.size();
  for (std::size_t i = 0; i < header.size(); ++i) {
    if (header[i] == field) {
      if (column != header.size()) {
        stop_with_error("Ambiguous CSV field name.");
      }
      column = i;
    }
  }
  if (column == header.size()) {
    stop_with_error("CSV field not found.");
  }
  values.clear();
  while (std::getline(input, line)) {
    const std::vector<std::string> row = split_csv_line(line);
    if (row.size() != header.size()) {
      stop_with_error("CSV row has the wrong number of fields.");
    }
    std::istringstream cell(row[column]);
    double value;
    if (!(cell >> value) || !std::isfinite(value)) {
      stop_with_error("CSV field must contain a finite number.");
    }
    cell >> std::ws;
    if (!cell.eof()) {
      stop_with_error("Unexpected text in numeric CSV field.");
    }
    values.push_back(value);
  }
  if (input.bad()) {
    stop_with_error("Cannot finish reading the CSV file.");
  }
  if (values.empty()) {
    stop_with_error("The CSV dataset must not be empty.");
  }
}
