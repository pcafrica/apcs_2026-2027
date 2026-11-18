#include <cstddef>
#include <iostream>
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

  Vector &operator=(const Vector &other) {
    if (this != &other) {
      Vector copy(other);
      std::swap(data_, copy.data_);
      std::swap(size_, copy.size_);
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

int main() {
  Vector source(3);
  source[0] = 1.25;

  Vector copied(source);
  copied[0] = 2.5;
  Vector assigned;
  assigned = source;

  std::cout << "Original: " << source[0] << '\n';   // 1.25
  std::cout << "Copy: " << copied[0] << '\n';       // 2.5
  std::cout << "Assigned: " << assigned[0] << '\n'; // 1.25

  // TODO: add the move constructor and move assignment to Vector.
  // TODO: check ownership transfer, then implement the assignment benchmark.
}
