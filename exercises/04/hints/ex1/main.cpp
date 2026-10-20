#include "data_source.hpp"
#include "data_transformer.hpp"
#include <iostream>

int main() {
  DataSource *data = nullptr;
  std::string input_type;
  std::cout << "Select input format (File | Console): ";
  if (!(std::cin >> input_type)) {
    std::cerr << "Error: Cannot read source type.\n";
    return 1;
  }
  if (input_type == "File") {
    // Run from the exercise root (04), as explained in README.md.
    // TODO 23: allocate a FileDataSource for "data.txt".
  } else if (input_type == "Console") {
    // TODO 24: allocate a ConsoleDataSource.
  } else {
    std::cerr << "Error: Invalid source type.\n";
    return 1;
  }
  if (data == nullptr) {
    stop_with_error("Complete source creation in main.cpp.");
  }
  // TODO 25: call display_info() and read_data() through the base pointer.
  std::cout << "Original data: " << *data << '\n';

  DataTransformer *transformer = nullptr;
  std::string transformation_type;
  std::cout << "Select transformation (Linear | Log | MinMax): ";
  if (!(std::cin >> transformation_type)) {
    delete data;
    std::cerr << "Error: Cannot read transformation type.\n";
    return 1;
  }
  if (transformation_type == "Linear") {
    // TODO 26: allocate a LinearScaler with factor 0.5.
  } else if (transformation_type == "Log") {
    // TODO 27: allocate a LogTransformer.
  } else if (transformation_type == "MinMax") {
    // TODO 28: allocate a MinMaxScaler.
  } else {
    delete data;
    std::cerr << "Error: Invalid transformation type.\n";
    return 1;
  }
  if (transformer == nullptr) {
    delete data;
    stop_with_error("Complete transformer creation in main.cpp.");
  }
  // TODO 29: call apply() through the base pointer.
  std::cout << "Transformed data: " << *data << '\n';
  // Destroy the transformer before its referenced source.
  // TODO 30: delete the transformer first, then the source.
  return 0;
}
