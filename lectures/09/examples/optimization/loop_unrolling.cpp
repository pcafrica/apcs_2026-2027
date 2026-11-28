#include <chrono>
#include <cstddef>
#include <iostream>
#include <vector>

double multiply(const std::vector<double> &data) {
  double result = 1.0;
  for (const auto value : data)
    result *= value;
  return result;
}

double multiply_with_unrolling(const std::vector<double> &data) {
  double result = 1.0;
  std::size_t i = 0;
  for (; data.size() - i >= 4; i += 4) {
    result *= data[i];
    result *= data[i + 1];
    result *= data[i + 2];
    result *= data[i + 3];
  }
  for (; i < data.size(); ++i)
    result *= data[i];
  return result;
}

// This version groups four values before multiplying the running result.
double multiply_grouped(const std::vector<double> &data) {
  double result = 1.0;
  std::size_t i = 0;
  for (; data.size() - i >= 4; i += 4)
    result *= data[i] * data[i + 1] * data[i + 2] * data[i + 3];
  for (; i < data.size(); ++i)
    result *= data[i];
  return result;
}

int main() {
  using clock = std::chrono::steady_clock;
  constexpr int repetitions = 5;
  std::vector<double> data(1'000'000, 1.00001);
  if (multiply(data) != multiply_with_unrolling(data))
    return 1;

  double ordinary_us = 0.0, unrolled_us = 0.0;
  double ordinary_result = 0.0, unrolled_result = 0.0;
  for (int repeat = 0; repeat < repetitions; ++repeat) {
    data.front() +=
        1e-9; // Both versions receive the same input in each repetition.
    auto start = clock::now();
    ordinary_result = multiply(data);
    ordinary_us +=
        std::chrono::duration<double, std::micro>(clock::now() - start).count();

    start = clock::now();
    unrolled_result = multiply_with_unrolling(data);
    unrolled_us +=
        std::chrono::duration<double, std::micro>(clock::now() - start).count();
    if (ordinary_result != unrolled_result)
      return 1;
  }

  std::cout << "Multiply: " << ordinary_us / repetitions
            << " us, result: " << ordinary_result << '\n';
  std::cout << "Unrolled: " << unrolled_us / repetitions
            << " us, result: " << unrolled_result << '\n';

  const std::vector<double> sensitive{1e-308, 1.0,   1.0,    1.0,
                                      1e308,  1e308, 1e-308, 1.0};
  std::cout << "Order-sensitive input: " << multiply(sensitive)
            << " (sequential), " << multiply_grouped(sensitive)
            << " (grouped)\n";
  return 0;
}
