#ifndef DATA_PROCESSOR_HPP
#define DATA_PROCESSOR_HPP

#include <ostream>

class DataProcessor {
public:
  DataProcessor(const double *input_data, unsigned int input_size);
  DataProcessor(const DataProcessor &other);
  DataProcessor &operator=(const DataProcessor &other);
  ~DataProcessor();

  DataProcessor operator+(const DataProcessor &other) const;

  double &operator[](unsigned int index);
  const double &operator[](unsigned int index) const;

  unsigned int n_elements() const;
  double min() const;
  double max() const;
  double compute_mean() const;
  double compute_std_dev() const;

  static unsigned int get_n_instances();

  friend std::ostream &operator<<(std::ostream &os, const DataProcessor &dp);

private:
  static unsigned int n_instances;
  unsigned int size;
  double *data;
};

double compute_correlation(const DataProcessor &dp1, const DataProcessor &dp2);

#endif // DATA_PROCESSOR_HPP
