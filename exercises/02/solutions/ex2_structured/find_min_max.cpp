#include <iostream>

// Function to find the minimum and maximum values in an array.
void find_min_max(const int *arr, int size, int &min_val, int &max_val) {
  if (size == 0) {
    std::cout << "Array is empty." << std::endl;
    return;
  }
  min_val = arr[0]; // Initialize min_val to the first element.
  max_val = arr[0]; // Initialize max_val to the first element.

  for (int i = 1; i < size; ++i) {
    if (arr[i] < min_val) {
      min_val = arr[i]; // Update min_val if a smaller element is found.
    } else if (arr[i] > max_val) {
      max_val = arr[i]; // Update max_val if a larger element is found.
    }
  }
}
