#ifndef NEWTON_HPP
#define NEWTON_HPP

#include "../newton_types.hpp"
#include <functional>

template <typename T> class NewtonSolver {
public:
  using Function = std::function<T(const T &)>;
  NewtonSolver(const Function &f, const Function &df, const T &x0,
               NewtonOptions options = {});
  NewtonResult<T> solve() const;

private:
  Function f;
  Function df;
  T x0;
  NewtonOptions options;
};

template <typename T>
NewtonSolver<T>::NewtonSolver(const Function &f, const Function &df,
                              const T &x0, NewtonOptions options)
    : f(f), df(df), x0(x0), options(options) {}

template <typename T> NewtonResult<T> NewtonSolver<T>::solve() const {
  NewtonResult<T> result{x0};
  if (!f || !df || !newton_is_finite(x0) ||
      !std::isfinite(options.residual_tolerance) ||
      options.residual_tolerance <= 0.0 ||
      !std::isfinite(options.step_tolerance) || options.step_tolerance <= 0.0 ||
      !std::isfinite(options.derivative_threshold) ||
      options.derivative_threshold < 0.0 || options.max_iterations == 0) {
    result.message = "invalid input or solver options";
    return result;
  }

  T fx = f(result.value);
  if (!newton_is_finite(fx) || !std::isfinite(std::abs(fx))) {
    result.message = "non-finite function value";
    return result;
  }
  result.residual = std::abs(fx);
  // Test the residual before the derivative: an exact root may have f'=0.
  if (result.residual <= options.residual_tolerance) {
    result.converged = true;
    result.message = "initial residual satisfies tolerance";
    return result;
  }

  for (unsigned int it = 0; it < options.max_iterations; ++it) {
    const T derivative = df(result.value);
    if (!newton_is_finite(derivative) || !std::isfinite(std::abs(derivative)) ||
        std::abs(derivative) <= options.derivative_threshold) {
      result.message = "derivative is non-finite or too small";
      return result;
    }
    const T delta = fx / derivative;
    const T next = result.value - delta;
    if (!newton_is_finite(delta) || !std::isfinite(std::abs(delta)) ||
        !newton_is_finite(next) || !std::isfinite(std::abs(next))) {
      result.message = "non-finite Newton update";
      return result;
    }
    result.value = next;
    result.iterations = it + 1;
    result.last_step = std::abs(delta);
    fx = f(result.value);
    if (!newton_is_finite(fx) || !std::isfinite(std::abs(fx))) {
      result.residual = std::numeric_limits<double>::infinity();
      result.message = "non-finite function value after update";
      return result;
    }
    result.residual = std::abs(fx);
    if (result.residual <= options.residual_tolerance) {
      result.converged = true;
      result.message = "residual satisfies tolerance";
      return result;
    }
    // A small step without a small residual is stagnation, not success.
    if (result.last_step <=
        options.step_tolerance * (1.0 + std::abs(result.value))) {
      result.message = "small step but residual is too large (stagnation)";
      return result;
    }
  }
  result.message = "iteration limit reached";
  return result;
}

#endif // NEWTON_HPP
