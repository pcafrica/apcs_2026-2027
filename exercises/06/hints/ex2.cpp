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

int main() {
  try {
    std::ifstream input("input.txt");
    if (!input) {
      throw std::runtime_error("Cannot open input.txt");
    }

    const auto measurements = read_measurements(input);
    std::cout << "Loaded " << measurements.size() << " sensor groups\n";

    // TODO: build a vector<Summary> with each sensor's count and mean.
    // TODO: sort by count descending, then sensor ID ascending.
    // TODO: write output.txt and check writing and closing for errors.
  } catch (const std::exception &error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }

  return 0;
}
