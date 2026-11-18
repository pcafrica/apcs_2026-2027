#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

std::vector<unsigned int> sorted_unique(std::vector<unsigned int> numbers) {
  std::sort(numbers.begin(), numbers.end());
  const auto new_end = std::unique(numbers.begin(), numbers.end());
  numbers.erase(new_end, numbers.end());
  return numbers;
}

std::vector<unsigned int>
sorted_unique_with_set(const std::vector<unsigned int> &numbers) {
  const std::set<unsigned int> unique_ids(numbers.begin(), numbers.end());
  return std::vector<unsigned int>(unique_ids.begin(), unique_ids.end());
}

std::vector<unsigned int>
first_occurrences(const std::vector<unsigned int> &numbers) {
  std::vector<unsigned int> result;
  std::unordered_set<unsigned int> seen;

  for (unsigned int id : numbers) {
    if (seen.count(id) == 0) {
      seen.insert(id);
      result.push_back(id);
    }
  }

  return result;
}

void print(const std::string &label, const std::vector<unsigned int> &numbers) {
  std::cout << label << ":\n";
  for (unsigned int id : numbers) {
    std::cout << id << ' ';
  }
  std::cout << "\n\n";
}

int main() {
  std::mt19937 engine{12345};
  std::uniform_int_distribution<unsigned int> distribution(0, 9);
  std::vector<unsigned int> random_numbers(100);

  std::generate(random_numbers.begin(), random_numbers.end(),
                [&]() { return distribution(engine); });

  auto sorted_numbers = random_numbers;
  std::sort(sorted_numbers.begin(), sorted_numbers.end());

  const auto sorted_unique_numbers = sorted_unique(random_numbers);
  const auto unsorted_unique_numbers = first_occurrences(random_numbers);

  print("Sampled sensor IDs", random_numbers);
  print("Sorted IDs", sorted_numbers);
  print("Sorted unique IDs", sorted_unique_numbers);
  print("First occurrences", unsorted_unique_numbers);
  print("Sorted unique IDs (set)", sorted_unique_with_set(random_numbers));
}
