#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <utility>

class Vector {
public:
  explicit Vector(std::size_t size = 0)
      : data_(size ? new double[size]{} : nullptr), size_(size) {}

  Vector(const Vector &other)
      : data_(other.size_ ? new double[other.size_] : nullptr),
        size_(other.size_) {
    for (std::size_t i = 0; i < size_; ++i) {
      data_[i] = other.data_[i];
    }
  }

  Vector(Vector &&other) noexcept : data_(other.data_), size_(other.size_) {
    other.data_ = nullptr;
    other.size_ = 0;
  }

  Vector &operator=(const Vector &other) {
    if (this != &other) {
      Vector copy(other);
      std::swap(data_, copy.data_);
      std::swap(size_, copy.size_);
    }
    return *this;
  }

  Vector &operator=(Vector &&other) noexcept {
    if (this != &other) {
      if (data_)
        delete[] data_;

      data_ = other.data_;
      size_ = other.size_;
      other.data_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  ~Vector() { delete[] data_; }

  std::size_t n_elements() const { return size_; }
  const double *storage() const { return data_; }

  // Indexing requires i < n_elements().
  double &operator[](std::size_t i) { return data_[i]; }
  const double &operator[](std::size_t i) const { return data_[i]; }

private:
  double *data_;
  std::size_t size_;
};

// Both measurement functions require size >= 2.
double measure_copy(std::size_t size) {
  const int repetitions = 20;
  double total_us = 0.0;

  for (int i = 0; i < repetitions; ++i) {
    Vector source(size);
    source[0] = 1.0;
    source[size - 1] = 2.0;
    Vector destination;

    const auto start = std::chrono::steady_clock::now();
    destination = source;
    const auto end = std::chrono::steady_clock::now();

    total_us += std::chrono::duration<double, std::micro>(end - start).count();
    if (destination.n_elements() != size || destination[0] != source[0] ||
        destination[size - 1] != source[size - 1]) {
      throw std::logic_error("Copy assignment failed");
    }
  }

  return total_us / repetitions;
}

double measure_move(std::size_t size) {
  const int repetitions = 20;
  double total_us = 0.0;

  for (int i = 0; i < repetitions; ++i) {
    Vector source(size);
    source[0] = 1.0;
    source[size - 1] = 2.0;
    Vector destination;

    const auto start = std::chrono::steady_clock::now();
    destination = std::move(source);
    const auto end = std::chrono::steady_clock::now();

    total_us += std::chrono::duration<double, std::micro>(end - start).count();
    if (destination.n_elements() != size || destination[0] != 1.0 ||
        destination[size - 1] != 2.0 || source.n_elements() != 0) {
      throw std::logic_error("Move assignment failed");
    }
  }

  return total_us / repetitions;
}

int main() {
  try {
    std::cout << "elements copy_us move_us speedup\n";
    std::cout << std::setprecision(6);

    for (std::size_t size = 1024; size <= 2'097'152; size *= 2) {
      const double copy_us = measure_copy(size);
      const double move_us = measure_move(size);

      std::cout << size << ' ' << copy_us << ' ' << move_us << ' '
                << copy_us / move_us << '\n';
    }
  } catch (const std::exception &error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }

  return 0;
}
