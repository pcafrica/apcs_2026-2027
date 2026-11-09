#ifndef NEWTON_HPP
#define NEWTON_HPP

#include "newton_types.hpp"
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

#endif // NEWTON_HPP
