#ifndef DATA_SOURCE_HPP
#define DATA_SOURCE_HPP

#include "data_io.hpp"
#include <ostream>
#include <string>

class DataSource {
public:
  DataSource(const std::string &name) : name(name) {}
  // TODO 1: declare a public virtual destructor.
  // TODO 2: declare a pure virtual void read_data() method.

  // Supplied indexed interface: no vector operations are needed.
  std::size_t n_elements() const { return data.n_elements(); }
  double &operator[](std::size_t index) { return data[index]; }
  const double &operator[](std::size_t index) const { return data[index]; }
  void display_info() const;
  friend std::ostream &operator<<(std::ostream &os, const DataSource &source);

protected:
  DataBuffer data;

private:
  std::string name;
};

class FileDataSource /* TODO 3: publicly inherit from DataSource */ {
public:
  FileDataSource(const std::string &filename);
  // TODO 4: declare the destructor, using override.
  void read_data() /* TODO 5: override */;

private:
  std::string filename;
};

class ConsoleDataSource /* TODO 6: publicly inherit from DataSource */ {
public:
  ConsoleDataSource();
  // TODO 7: declare the destructor, using override.
  void read_data() /* TODO 8: override */;
};

#endif // DATA_SOURCE_HPP
