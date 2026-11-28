#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <vector>

class Matrix {
public:
  Matrix(std::size_t size) : rows(size), cols(size), data(rows * cols, 0.0) {}

  Matrix(std::size_t rows, std::size_t cols)
      : rows(rows), cols(cols), data(rows * cols, 0.0) {}

  Matrix(const Matrix &) = default;

  std::size_t n_rows() const { return rows; }
  std::size_t n_cols() const { return cols; }

  const double &operator()(std::size_t i, std::size_t j) const {
    return data[sub2ind(i, j)];
  }

  double &operator()(std::size_t i, std::size_t j) {
    return data[sub2ind(i, j)];
  }

  // Returns a new transposed matrix; leaves this matrix unchanged.
  Matrix transpose() const;

private:
  const std::size_t rows;
  const std::size_t cols;
  std::vector<double> data;

  inline std::size_t sub2ind(const std::size_t i, const std::size_t j) const {
    return (i + j * rows);
  }
};

// Matrix-matrix product: C = A * B.
Matrix operator*(const Matrix &A, const Matrix &B);

// Matrix-matrix product with a transposed first factor.
Matrix multiply_transposed(const Matrix &A, const Matrix &B);

// Matrix-vector product: y = A * x.
std::vector<double> operator*(const Matrix &A, const std::vector<double> &x);

#endif /* MATRIX_HPP */
