#include "data_processor.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <numeric>

void report_error(const char *message) {
  std::cerr << "Error: " << message << '\n';
  std::exit(EXIT_FAILURE);
}

unsigned int DataProcessor::n_instances = 0;

DataProcessor::DataProcessor(const double *input_data, unsigned int input_size)
    : size(input_size), data(nullptr) {
  if (size == 0 || input_data == nullptr) {
    report_error("DataProcessor requires a non-empty array");
  }
  data = new double[size];
  std::copy_n(input_data, size, data);
  ++n_instances;
}

DataProcessor::DataProcessor(const DataProcessor &other)
    : size(other.size), data(new double[size]) {
  std::copy_n(other.data, size, data);
  ++n_instances;
}

DataProcessor &DataProcessor::operator=(const DataProcessor &other) {
  if (this != &other) {
    double *new_data = new double[other.size];
    std::copy_n(other.data, other.size, new_data);
    delete[] data;
    data = new_data;
    size = other.size;
  }
  return *this;
}

DataProcessor::~DataProcessor() {
  delete[] data;
  --n_instances;
}

DataProcessor DataProcessor::operator+(const DataProcessor &other) const {
  if (size != other.size) {
    report_error("Cannot add datasets of different sizes");
  }
  DataProcessor result(*this);
  for (unsigned int i = 0; i < size; ++i) {
    result[i] += other[i];
  }
  return result;
}

double &DataProcessor::operator[](unsigned int index) {
  if (index >= size) {
    report_error("DataProcessor index out of range");
  }
  return data[index];
}

const double &DataProcessor::operator[](unsigned int index) const {
  if (index >= size) {
    report_error("DataProcessor index out of range");
  }
  return data[index];
}

unsigned int DataProcessor::n_elements() const { return size; }

double DataProcessor::min() const {
  return *std::min_element(data, data + size);
}

double DataProcessor::max() const {
  return *std::max_element(data, data + size);
}

double DataProcessor::compute_mean() const {
  return std::accumulate(data, data + size, 0.0) / size;
}

double DataProcessor::compute_std_dev() const {
  const double mean = compute_mean();
  double sum_squared_diff = 0.0;
  for (unsigned int i = 0; i < size; ++i) {
    const double diff = data[i] - mean;
    sum_squared_diff += diff * diff;
  }
  return std::sqrt(sum_squared_diff / size); // Population standard deviation.
}

unsigned int DataProcessor::get_n_instances() { return n_instances; }

std::ostream &operator<<(std::ostream &os, const DataProcessor &dp) {
  for (unsigned int i = 0; i < dp.size; ++i) {
    if (i != 0) {
      os << ", ";
    }
    os << dp[i];
  }
  return os;
}

double compute_correlation(const DataProcessor &dp1, const DataProcessor &dp2) {
  if (dp1.n_elements() != dp2.n_elements()) {
    report_error("Cannot correlate datasets of different sizes");
  }

  const double mean1 = dp1.compute_mean();
  const double mean2 = dp2.compute_mean();
  double cross_products = 0.0;
  double sum_diff1_squared = 0.0;
  double sum_diff2_squared = 0.0;

  for (unsigned int i = 0; i < dp1.n_elements(); ++i) {
    const double diff1 = dp1[i] - mean1;
    const double diff2 = dp2[i] - mean2;
    cross_products += diff1 * diff2;
    sum_diff1_squared += diff1 * diff1;
    sum_diff2_squared += diff2 * diff2;
  }
  if (sum_diff1_squared == 0.0 || sum_diff2_squared == 0.0) {
    report_error("Correlation requires two non-constant datasets");
  }
  return cross_products /
         (std::sqrt(sum_diff1_squared) * std::sqrt(sum_diff2_squared));
}
