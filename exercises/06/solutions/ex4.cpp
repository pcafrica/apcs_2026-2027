#include <iostream>
#include <string>

template <class T1, class T2> class MyPair {
public:
  T1 first;
  T2 second;

  MyPair(const T1 &a = T1{}, const T2 &b = T2{}) : first(a), second(b) {}

  bool operator==(const MyPair &other) const {
    return first == other.first && second == other.second;
  }

  bool operator!=(const MyPair &other) const { return !(*this == other); }

  bool operator<(const MyPair &other) const {
    return first < other.first ||
           (!(other.first < first) && second < other.second);
  }
};

int main() {
  const MyPair<int, double> defaults;
  const MyPair<int, int> a{1, 9};
  const MyPair<int, int> b{2, 0};
  const MyPair<int, int> c{1, 10};
  const MyPair<double, std::string> reading{1.25, "sensor"};
  const MyPair<double, std::string> copy = reading;
  const MyPair<int, std::string> label1{1, "alpha"};
  const MyPair<int, std::string> label2{1, "beta"};

  if (defaults.first != 0 || defaults.second != 0.0 || !(a < b) || b < a ||
      !(a < c) || a < a || !(a == a) || a != a || !(a != b) ||
      !(reading == copy) || reading != copy || !(label1 < label2)) {
    std::cerr << "MyPair comparison or initialization failed\n";
    return 1;
  }

  std::cout << "All MyPair checks passed.\n";
}
