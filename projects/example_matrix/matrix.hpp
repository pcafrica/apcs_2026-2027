#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

template <class T> class Matrix {
public:
  Matrix(const std::size_t r, const std::size_t c, const T &value = T{});

  std::size_t n_rows() const;
  std::size_t n_cols() const;
  std::size_t size() const;

  T &operator()(const std::size_t r, const std::size_t c);
  const T &operator()(const std::size_t r, const std::size_t c) const;

  typename std::vector<T>::iterator begin();
  typename std::vector<T>::iterator end();
  typename std::vector<T>::const_iterator begin() const;
  typename std::vector<T>::const_iterator end() const;

private:
  std::size_t rows, cols;
  std::vector<T> values;

  static std::size_t check_size(const std::size_t r, const std::size_t c);
  std::size_t index(const std::size_t r, const std::size_t c) const;

  // std::vector manages storage; no custom destructor, copy or move.
};

template <class T> void print(const Matrix<T> &a) {
  for (std::size_t r = 0; r < a.n_rows(); ++r) {
    for (std::size_t c = 0; c < a.n_cols(); ++c) {
      // Unary + prints uint8_t numerically instead of as a character.
      std::cout << +a(r, c) << ' ';
    }
    std::cout << '\n';
  }
}

#endif // MATRIX_HPP
