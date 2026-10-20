#include "data_transformer.hpp"
#include <cmath>

// TODO 19: define the derived constructors, initializing the base class.
// LinearScaler also needs to initialize scaling_factor.

void LinearScaler::apply() const {
  // TODO 20: reject empty input with stop_with_error("...").
  // Multiply source[i] by scaling_factor in an indexed loop.
  stop_with_error("Complete LinearScaler::apply().");
}

void LogTransformer::apply() const {
  // TODO 21: reject empty input. Apply std::log to positive values;
  // replace values <= 0 with 0.
  stop_with_error("Complete LogTransformer::apply().");
}

void MinMaxScaler::apply() const {
  // TODO 22: reject empty input before accessing source[0].
  // Find min and max with an indexed loop. Map constants to all zeros;
  // otherwise apply (value - min) / (max - min).
  stop_with_error("Complete MinMaxScaler::apply().");
}
