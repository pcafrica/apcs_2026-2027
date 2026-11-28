#include <cstddef>
#include <iostream>

class Class1 {
  short int s;
  int i;
  char c;
};

class Class2 {
  int i;
  short int s;
  char c;
};

int main() {
  constexpr std::size_t length = 100'000'000;
  constexpr double bytes_per_gib = 1024.0 * 1024.0 * 1024.0;

  std::cout << "Class1: sizeof = " << sizeof(Class1)
            << ", alignof = " << alignof(Class1) << '\n';
  std::cout << "Class2: sizeof = " << sizeof(Class2)
            << ", alignof = " << alignof(Class2) << '\n';
  std::cout << "Estimated element storage for " << length << " objects:\n";
  std::cout << "Class1: "
            << length * static_cast<double>(sizeof(Class1)) / bytes_per_gib
            << " GiB\n";
  std::cout << "Class2: "
            << length * static_cast<double>(sizeof(Class2)) / bytes_per_gib
            << " GiB\n";
  return 0;
}
