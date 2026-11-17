#include "matrix.hpp"

#include <limits>
#include <stdexcept>

template <class T>
std::size_t Matrix<T>::check_size(const std::size_t r, const std::size_t c) {
  if (r == 0 || c == 0) {
    throw std::invalid_argument("Matrix dimensions must be positive.");
  }

  if (r > std::numeric_limits<std::size_t>::max() / c) {
    throw std::length_error("Matrix dimensions overflow.");
  }

  return r * c;
}

template <class T>
Matrix<T>::Matrix(const std::size_t r, const std::size_t c, const T &value)
    : rows(r), cols(c), values(check_size(r, c), value) {}

template <class T> std::size_t Matrix<T>::n_rows() const { return rows; }

template <class T> std::size_t Matrix<T>::n_cols() const { return cols; }

template <class T> std::size_t Matrix<T>::size() const { return values.size(); }

template <class T>
std::size_t Matrix<T>::index(const std::size_t r, const std::size_t c) const {
  if (r >= rows || c >= cols) {
    throw std::out_of_range("Matrix index out of range.");
  }

  // Row-major storage: consecutive columns are adjacent in memory.
  return r * cols + c;
}

template <class T>
T &Matrix<T>::operator()(const std::size_t r, const std::size_t c) {
  return values.at(index(r, c));
}

template <class T>
const T &Matrix<T>::operator()(const std::size_t r, const std::size_t c) const {
  return values.at(index(r, c));
}

// Iteration follows the flat row-major storage order.
template <class T> typename std::vector<T>::iterator Matrix<T>::begin() {
  return values.begin();
}

template <class T> typename std::vector<T>::iterator Matrix<T>::end() {
  return values.end();
}

template <class T>
typename std::vector<T>::const_iterator Matrix<T>::begin() const {
  return values.begin();
}

template <class T>
typename std::vector<T>::const_iterator Matrix<T>::end() const {
  return values.end();
}

// Generate the implementations for these two types.
template class Matrix<double>;
template class Matrix<std::uint8_t>;
