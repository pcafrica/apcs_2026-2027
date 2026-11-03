#ifndef NEWTON_TYPES_HPP
#define NEWTON_TYPES_HPP

#include <cmath>
#include <complex>
#include <limits>

struct NewtonOptions {
  double residual_tolerance = 1e-12;
  double step_tolerance = 1e-12;
  double derivative_threshold = 1e-14;
  unsigned int max_iterations = 100;
};

template <typename T> struct NewtonResult {
  T value;
  bool converged = false;
  unsigned int iterations = 0; // Number of completed finite updates.
  double residual = std::numeric_limits<double>::infinity();
  double last_step = 0.0;
  const char *message = "not started";
};

inline bool newton_is_finite(double value) { return std::isfinite(value); }

inline bool newton_is_finite(const std::complex<double> &value) {
  return std::isfinite(value.real()) && std::isfinite(value.imag());
}

#endif // NEWTON_TYPES_HPP
