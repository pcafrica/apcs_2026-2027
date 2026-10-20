#include "data_source.hpp"
#include <iostream>

void DataSource::display_info() const {
  std::cout << "Data source name: " << name << '\n';
}

void FileDataSource::read_data() { data.read_file(filename); }
void ConsoleDataSource::read_data() { data.read_console(); }

std::ostream &operator<<(std::ostream &os, const DataSource &source) {
  for (std::size_t i = 0; i < source.n_elements(); ++i) {
    if (i != 0) {
      os << ", ";
    }
    os << source[i];
  }
  return os;
}
