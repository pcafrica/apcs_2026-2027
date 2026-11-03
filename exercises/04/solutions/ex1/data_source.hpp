#ifndef DATA_SOURCE_HPP
#define DATA_SOURCE_HPP

#include "data_io.hpp"
#include <ostream>
#include <string>

class DataSource {
public:
  DataSource(const std::string &name) : name(name) {}
  virtual ~DataSource() = default;
  virtual void read_data() = 0;

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

class FileDataSource : public DataSource {
public:
  FileDataSource(const std::string &filename)
      : DataSource("File"), filename(filename) {}
  ~FileDataSource() override = default;
  void read_data() override;

private:
  std::string filename;
};

class ConsoleDataSource : public DataSource {
public:
  ConsoleDataSource() : DataSource("Console") {}
  ~ConsoleDataSource() override = default;
  void read_data() override;
};

#endif // DATA_SOURCE_HPP
