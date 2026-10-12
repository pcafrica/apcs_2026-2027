#include "statistics.hpp"

#include <algorithm>
#include <cmath>

namespace stat {

double calculate_mean(const double *numbers, int size) {
  double sum = 0.0;
  for (int i = 0; i < size; ++i) {
    sum += numbers[i];
  }

  return sum / size;
}

double calculate_standard_deviation(const double *numbers, int size) {
  const double mean = calculate_mean(numbers, size);

  double variance = 0.0;
  for (int i = 0; i < size; ++i) {
    const double difference = numbers[i] - mean;
    variance += difference * difference;
  }

  return std::sqrt(variance / size);
}

// Bonus: calculate the median without modifying the input array.
double calculate_median(const double *numbers, int size) {
  double *sorted_numbers = new double[size];

  for (int i = 0; i < size; ++i) {
    sorted_numbers[i] = numbers[i];
  }

  std::sort(sorted_numbers, sorted_numbers + size);

  double median;
  if (size % 2 == 0) {
    const int mid = size / 2;
    median = (sorted_numbers[mid - 1] + sorted_numbers[mid]) / 2.0;
  } else {
    median = sorted_numbers[size / 2];
  }

  delete[] sorted_numbers;

  return median;
}

} // namespace stat
