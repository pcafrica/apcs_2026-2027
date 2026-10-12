#include "find_min_max.hpp"

#include <cstdlib> // For rand() and srand().
#include <ctime>   // For time().
#include <iostream>

int main() {
  int size = 0;

  // Prompt the user for the size of the array.
  std::cout << "Enter the size of the array: ";
  if (!(std::cin >> size) || size <= 0) {
    std::cout << "Invalid array size. Please enter a positive integer."
              << std::endl;

    return 1; // Exit with an error code.
  }

  // Dynamically allocate memory for the array.
  int *arr = new int[size];

  // Seed the random number generator with the current time.
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  // Fill the array with random integers.
  for (int i = 0; i < size; ++i) {
    arr[i] = std::rand() % 100; // Generates random integers between 0 and 99.
  }

  // Find and display the maximum and minimum values in the array.
  int min_val, max_val;
  find_min_max(arr, size, min_val, max_val);

  std::cout << "Array elements:" << std::endl;
  for (int i = 0; i < size; ++i) {
    std::cout << arr[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "Maximum value: " << max_val << std::endl;
  std::cout << "Minimum value: " << min_val << std::endl;

  // Deallocate the dynamically allocated memory.
  // If forgotten, a memory leak will occur.
  delete[] arr;
  arr = nullptr;

  return 0;
}
