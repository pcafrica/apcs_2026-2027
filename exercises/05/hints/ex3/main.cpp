#include "newton.hpp"
#include <iostream>

int main() {
  // TODO: define f and df for x*x-1.
  const NewtonSolver solver(f, df, 0.5);
  const auto result = solver.solve();
  if (result.converged) {
    std::cout << "Approximate root: " << result.value << '\n';
  } else {
    std::cerr << "Solver failed: " << result.message << '\n';
  }
  std::cout << "Iterations: " << result.iterations
            << "; residual: " << result.residual << '\n';
  return result.converged ? 0 : 1;
}
