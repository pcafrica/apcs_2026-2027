#include <chrono>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

constexpr std::size_t vec_size = 1000;

std::vector<int> make_data() {
  std::vector<int> data(vec_size);
  std::iota(data.begin(), data.end(), 0);
  return data;
}

int lookup(std::size_t index) {
  const auto data = make_data();
  return data[index];
}

int lookup_static(std::size_t index) {
  static const auto data = make_data();
  return data[index];
}

int main() {
  using clock = std::chrono::steady_clock;
  constexpr int repetitions = 5;
  std::mt19937 engine{12345};
  std::uniform_int_distribution<std::size_t> index(0, vec_size - 1);
  std::vector<std::size_t> indices(20'000);
  for (auto &i : indices)
    i = index(engine);

  // Check all entries and initialize the static dataset before timing access.
  for (std::size_t i = 0; i < vec_size; ++i) {
    if (lookup(i) != lookup_static(i))
      return 1;
  }

  double ordinary_ms = 0.0, static_ms = 0.0;
  long long ordinary_total = 0, static_total = 0;
  for (int repeat = 0; repeat < repetitions; ++repeat) {
    indices.front() = index(engine); // Input preparation is outside the timing.
    auto start = clock::now();
    for (const auto i : indices)
      ordinary_total += lookup(i);
    ordinary_ms +=
        std::chrono::duration<double, std::milli>(clock::now() - start).count();

    start = clock::now();
    for (const auto i : indices)
      static_total += lookup_static(i);
    static_ms +=
        std::chrono::duration<double, std::milli>(clock::now() - start).count();
  }

  std::cout << "Lookup: " << ordinary_ms / repetitions
            << " ms, total: " << ordinary_total << '\n';
  std::cout << "Lookup (static): " << static_ms / repetitions
            << " ms, total: " << static_total << '\n';
  return ordinary_total == static_total ? 0 : 1;
}
