#include <iostream>
#include <limits>

int main() {
  const int largest = std::numeric_limits<int>::max();
  for (int i = 1; i <= 10; ++i) {
    // Convert before addition so that the addition is floating-point.
    // Remove static_cast<double> to demonstrate signed integer overflow.
    const double y = (static_cast<double>(i) + largest) / 1.0e10;
    std::cout << y << '\n';
  }
  return 0;
}
