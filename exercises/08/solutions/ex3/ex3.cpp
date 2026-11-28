#include "matrix.hpp"

#include <chrono>
#include <cstddef>
#include <iostream>
#include <stdexcept>

#ifndef MATRIX_SIZE
#define MATRIX_SIZE 200
#endif

bool equal_entries(const Matrix &a, const Matrix &b) {
  if (a.n_rows() != b.n_rows() || a.n_cols() != b.n_cols())
    return false;
  for (std::size_t i = 0; i < a.n_rows(); ++i)
    for (std::size_t j = 0; j < a.n_cols(); ++j)
      if (a(i, j) != b(i, j))
        return false;
  return true;
}

bool check_small_example() {
  Matrix a(2, 3), b(3, 2);
  double value = 1.0;
  for (std::size_t i = 0; i < 2; ++i)
    for (std::size_t j = 0; j < 3; ++j)
      a(i, j) = value++;
  value = 7.0;
  for (std::size_t i = 0; i < 3; ++i)
    for (std::size_t j = 0; j < 2; ++j)
      b(i, j) = value++;

  const Matrix t = a.transpose();
  if (t.n_rows() != 3 || t.n_cols() != 2)
    return false;
  for (std::size_t i = 0; i < 2; ++i)
    for (std::size_t j = 0; j < 3; ++j)
      if (t(j, i) != a(i, j))
        return false;

  const Matrix c = a * b;
  if (c.n_rows() != 2 || c.n_cols() != 2)
    return false;
  if (c(0, 0) != 58 || c(0, 1) != 64 || c(1, 0) != 139 || c(1, 1) != 154)
    return false;
  if (!equal_entries(c, multiply_transposed(a, b)))
    return false;
  const auto y = a * std::vector<double>{1, 2, 3};
  return y[0] == 14 && y[1] == 32;
}

using Product = Matrix (*)(const Matrix &, const Matrix &);

double mean_time(Product product, const Matrix &a, const Matrix &b,
                 const Matrix &expected) {
  constexpr int repetitions = 3;
  double total_ms = 0.0;
  for (int repeat = 0; repeat < repetitions; ++repeat) {
    const auto start = std::chrono::steady_clock::now();
    const Matrix result = product(a, b);
    const auto end = std::chrono::steady_clock::now();
    total_ms += std::chrono::duration<double, std::milli>(end - start).count();
    // These integer-valued inputs give exactly representable products and sums.
    if (!equal_entries(result, expected))
      throw std::runtime_error("Matrix product check failed");
  }
  return total_ms / repetitions;
}

int main() {
  if (!check_small_example()) {
    std::cerr << "The small rectangular example failed. Complete the matrix "
                 "methods.\n";
    return 1;
  }

  constexpr std::size_t size = MATRIX_SIZE;
  Matrix a(size), b(size);
  for (std::size_t i = 0; i < size; ++i) {
    a(i, i) = 10.0;
    a(i, size - 1) = 30.0;
    b(i, 0) = 1.0;
    b(i, i) = 3.0;
  }

  // Compute and check both versions before measuring them.
  const Matrix expected = a * b;
  if (!equal_entries(expected, multiply_transposed(a, b)))
    return 1;
  const double basic_ms = mean_time(operator*, a, b, expected);
  const double transposed_ms = mean_time(multiply_transposed, a, b, expected);
  std::cout << "Size: " << size << " x " << size << '\n';
  std::cout << "Baseline: " << basic_ms << " ms\n";
  std::cout << "With transpose (including its cost): " << transposed_ms
            << " ms\n";
  if (transposed_ms > 0.0)
    std::cout << "Speedup: " << basic_ms / transposed_ms << '\n';
  if (size > 0)
    std::cout << "First product entry: " << expected(0, 0) << '\n';
  return 0;
}
