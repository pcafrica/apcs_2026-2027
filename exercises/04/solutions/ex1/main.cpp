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
    data = new FileDataSource("data.txt");
  } else if (input_type == "Console") {
    data = new ConsoleDataSource();
  } else {
    std::cerr << "Error: Invalid source type.\n";
    return 1;
  }
  data->display_info();
  data->read_data();
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
    transformer = new LinearScaler(*data, 0.5);
  } else if (transformation_type == "Log") {
    transformer = new LogTransformer(*data);
  } else if (transformation_type == "MinMax") {
    transformer = new MinMaxScaler(*data);
  } else {
    delete data;
    std::cerr << "Error: Invalid transformation type.\n";
    return 1;
  }
  transformer->apply();
  std::cout << "Transformed data: " << *data << '\n';
  // Destroy the transformer before its referenced source.
  delete transformer;
  delete data;
  return 0;
}
