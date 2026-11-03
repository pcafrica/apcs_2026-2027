#ifndef NEWTON_HPP
#define NEWTON_HPP

#include "newton_types.hpp"
#include <functional>

// First complete this real-valued version; then make the solver a class
// template.
class NewtonSolver {
public:
  using Function = /* TODO: wrapper for double(const double &) */;
  NewtonSolver(const Function &f, const Function &df, double x0,
               NewtonOptions options = {});
  NewtonResult<double> solve() const;

private:
  Function f;
  Function df;
  double x0;
  NewtonOptions options;
};

#endif // NEWTON_HPP
