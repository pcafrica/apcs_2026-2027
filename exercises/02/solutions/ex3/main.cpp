#include "statistics.hpp"

#include <iostream>

int main() {
  std::cout << "Statistics calculator (using pointers)" << std::endl;

  while (true) {
    int size = 0;
    std::cout << "How many numbers? Enter 0 to quit: ";

    if (!(std::cin >> size)) {
      std::cerr << "Invalid input." << std::endl;
      return 1;
    }

    if (size == 0) {
      break;
    }

    if (size < 0) {
      std::cerr << "The size must be positive." << std::endl;
      continue;
    }

    double *numbers = new double[size];

    std::cout << "Enter " << size << (size == 1 ? " number: " : " numbers: ");
    for (int i = 0; i < size; ++i) {
      if (!(std::cin >> numbers[i])) {
        std::cerr << "Invalid number." << std::endl;
        delete[] numbers;
        return 1;
      }
    }

    const double mean = stat::calculate_mean(numbers, size);
    const double stddev = stat::calculate_standard_deviation(numbers, size);
    const double median = stat::calculate_median(numbers, size); // Bonus.

    std::cout << "Mean: " << mean << std::endl;
    std::cout << "Population standard deviation: " << stddev << std::endl;
    std::cout << "Median (bonus): " << median << std::endl;

    delete[] numbers;
    numbers = nullptr;
  }

  return 0;
}
