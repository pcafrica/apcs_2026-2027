#include "data_transformer.hpp"
#include <cmath>

void LinearScaler::apply() const {
  if (source.n_elements() == 0) {
    stop_with_error("Read a non-empty dataset before transforming it.");
  }
  for (std::size_t i = 0; i < source.n_elements(); ++i) {
    source[i] *= scaling_factor;
  }
}

void LogTransformer::apply() const {
  if (source.n_elements() == 0) {
    stop_with_error("Read a non-empty dataset before transforming it.");
  }
  for (std::size_t i = 0; i < source.n_elements(); ++i) {
    source[i] = source[i] > 0.0 ? std::log(source[i]) : 0.0;
  }
}

void MinMaxScaler::apply() const {
  if (source.n_elements() == 0) {
    stop_with_error("Read a non-empty dataset before transforming it.");
  }
  double minimum = source[0];
  double maximum = source[0];
  for (std::size_t i = 1; i < source.n_elements(); ++i) {
    if (source[i] < minimum) {
      minimum = source[i];
    }
    if (source[i] > maximum) {
      maximum = source[i];
    }
  }
  for (std::size_t i = 0; i < source.n_elements(); ++i) {
    source[i] =
        maximum == minimum ? 0.0 : (source[i] - minimum) / (maximum - minimum);
  }
}
