#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct Summary {
  int sensor;
  std::size_t count;
  double mean;
};

std::map<int, std::vector<double>> read_measurements(std::istream &input) {
  std::map<int, std::vector<double>> measurements;
  std::string line;
  std::size_t line_number = 0;

  while (std::getline(input, line)) {
    ++line_number;
    if (line.find_first_not_of(" \t\r") == std::string::npos) {
      continue;
    }

    std::istringstream row(line);
    int sensor = 0;
    double value = 0.0;
    std::string extra;

    if (!(row >> sensor >> value) || sensor < 0 || !std::isfinite(value) ||
        (row >> extra)) {
      throw std::runtime_error("Invalid measurement on line " +
                               std::to_string(line_number));
    }

    measurements[sensor].push_back(value);
  }

  if (input.bad()) {
    throw std::runtime_error("Reading measurements failed");
  }

  return measurements;
}

std::vector<Summary>
summarize(const std::map<int, std::vector<double>> &measurements) {
  std::vector<Summary> summaries;

  for (const auto &[sensor, values] : measurements) {
    if (values.empty()) {
      throw std::invalid_argument("A sensor group must contain a measurement");
    }

    const double sum = std::accumulate(values.begin(), values.end(), 0.0);
    if (!std::isfinite(sum)) {
      throw std::runtime_error("Non-finite sum for sensor " +
                               std::to_string(sensor));
    }

    summaries.push_back(
        {sensor, values.size(), sum / static_cast<double>(values.size())});
  }

  std::sort(summaries.begin(), summaries.end(),
            [](const Summary &a, const Summary &b) {
              if (a.count != b.count) {
                return a.count > b.count;
              }
              return a.sensor < b.sensor;
            });

  return summaries;
}

void write_summaries(std::ostream &output,
                     const std::vector<Summary> &summaries) {
  output << std::setprecision(12);
  for (const auto &summary : summaries) {
    output << summary.sensor << ' ' << summary.count << ' ' << summary.mean
           << '\n';
  }

  if (!output) {
    throw std::runtime_error("Writing summaries failed");
  }
}

int main() {
  try {
    std::ifstream input("input.txt");
    if (!input) {
      throw std::runtime_error("Cannot open input.txt");
    }

    const auto measurements = read_measurements(input);
    const auto summaries = summarize(measurements);

    std::ofstream output("output.txt");
    if (!output) {
      throw std::runtime_error("Cannot open output.txt");
    }

    write_summaries(output, summaries);
    output.close();
    if (!output) {
      throw std::runtime_error("Closing output.txt failed");
    }

    std::cout << "Written " << summaries.size()
              << " sensor summaries to output.txt\n";
  } catch (const std::exception &error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }

  return 0;
}
