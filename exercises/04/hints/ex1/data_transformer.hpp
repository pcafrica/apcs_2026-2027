#ifndef DATA_TRANSFORMER_HPP
#define DATA_TRANSFORMER_HPP

#include "data_source.hpp"

class DataTransformer {
public:
  // TODO 11: constructor receiving DataSource& and initializing source.
  // TODO 12: public virtual destructor and pure virtual apply() const.
protected:
  // Non-owning: the source must outlive this transformer.
  DataSource &source;
};

class LinearScaler /* TODO 13: public inheritance */ {
public:
  LinearScaler(DataSource &source, double scaling_factor);
  void apply() const /* TODO 14: override */;

private:
  double scaling_factor;
};

class LogTransformer /* TODO 15: public inheritance */ {
public:
  LogTransformer(DataSource &source);
  void apply() const /* TODO 16: override */;
};

class MinMaxScaler /* TODO 17: public inheritance */ {
public:
  MinMaxScaler(DataSource &source);
  void apply() const /* TODO 18: override */;
};

#endif // DATA_TRANSFORMER_HPP
