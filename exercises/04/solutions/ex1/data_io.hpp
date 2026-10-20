#ifndef DATA_IO_HPP
#define DATA_IO_HPP

// Supplied support code: no knowledge of STL containers or file streams
// required.
#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector>

void stop_with_error(const char *message);

class DataBuffer {
public:
  void read_file(const std::string &filename);
  void read_console();
  void read_csv(const std::string &filename, const std::string &field);
  std::size_t n_elements() const;
  double &operator[](std::size_t index);
  const double &operator[](std::size_t index) const;

private:
  std::vector<double> values;
};

#endif // DATA_IO_HPP
