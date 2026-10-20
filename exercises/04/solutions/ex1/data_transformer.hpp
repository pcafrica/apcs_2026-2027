#ifndef DATA_TRANSFORMER_HPP
#define DATA_TRANSFORMER_HPP

#include "data_source.hpp"

class DataTransformer {
public:
  DataTransformer(DataSource &source) : source(source) {}
  virtual ~DataTransformer() = default;
  virtual void apply() const = 0;

protected:
  // Non-owning reference: the source must outlive this transformer.
  DataSource &source;
};

class LinearScaler : public DataTransformer {
public:
  LinearScaler(DataSource &source, double scaling_factor)
      : DataTransformer(source), scaling_factor(scaling_factor) {}
  void apply() const override;

private:
  double scaling_factor;
};

class LogTransformer : public DataTransformer {
public:
  LogTransformer(DataSource &source) : DataTransformer(source) {}
  void apply() const override;
};

class MinMaxScaler : public DataTransformer {
public:
  MinMaxScaler(DataSource &source) : DataTransformer(source) {}
  void apply() const override;
};

#endif // DATA_TRANSFORMER_HPP
