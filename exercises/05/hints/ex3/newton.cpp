#include "newton.hpp"

NewtonSolver::NewtonSolver(const Function &f, const Function &df, double x0,
                           NewtonOptions options)
    : f(f), df(df), x0(x0), options(options) {}

NewtonResult<double> NewtonSolver::solve() const {
  NewtonResult<double> result{x0};
  // TODO 1: validate callbacks, finite x0 and options (see slides/README).
  // TODO 2: evaluate f(x0), reject non-finite values, store |f(x0)|.
  //         If the residual is small enough, succeed without evaluating df.
  // TODO 3: for at most max_iterations updates:
  //   - evaluate df(x); reject a non-finite or too-small derivative BEFORE
  //   division;
  //   - compute delta=f(x)/df(x) and next=x-delta; reject non-finite values;
  //   - store next, the completed-update count and |delta|;
  //   - evaluate f(next), reject non-finite values, and store its residual;
  //   - succeed ONLY if the residual meets residual_tolerance;
  //   - otherwise fail for stagnation if |delta| <= step_tolerance*(1+|next|).
  // TODO 4: if the iteration limit is reached, return a failed result.
  // Set message on every exit; do not compare results with NaN.
  result.message = "complete the Newton iteration";
  return result;
}
