#include "newton.hpp"

#include <complex>
#include <iomanip>
#include <iostream>

template <typename T>
void print_result(const NewtonResult<T> &result) {
  std::cout << (result.converged ? "Converged" : "Failed")
            << ": value=" << result.value
            << ", iterations=" << result.iterations
            << ", residual=" << result.residual
            << ", last step=" << result.last_step
            << "\nReason: " << result.message << "\n\n";
}

int main() {
  std::cout << std::setprecision(16);
  const auto f = [](const double &x) { return x * x - 1.0; };
  const auto df = [](const double &x) { return 2.0 * x; };
  const NewtonSolver<double> real_solver(f, df, 0.5);
  const auto real_result = real_solver.solve();
  print_result(real_result);

  const auto g = [](const std::complex<double> &z) { return z * z + 1.0; };
  const auto dg = [](const std::complex<double> &z) { return 2.0 * z; };
  const NewtonSolver<std::complex<double>> complex_solver(g, dg, {0.5, 0.5});
  const auto complex_result = complex_solver.solve();
  print_result(complex_result);
  return real_result.converged && complex_result.converged ? 0 : 1;
}
