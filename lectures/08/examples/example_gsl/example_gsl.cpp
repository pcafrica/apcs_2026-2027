#include <gsl/gsl_errno.h>
#include <gsl/gsl_integration.h>

#include <cmath>
#include <iostream>

// GSL calls this function at the integration points.
double my_function(double x, void *) { return std::sin(x); }

int main() {
  // Handle GSL failures by checking the returned status.
  gsl_set_error_handler_off();
  gsl_integration_workspace *workspace = gsl_integration_workspace_alloc(1000);
  if (!workspace) {
    std::cerr << "Cannot allocate integration workspace\n";
    return 1;
  }

  double result = 0.0, error = 0.0;
  gsl_function F;
  F.function = &my_function;
  F.params = nullptr;

  const double lower_limit = 0.0;
  const double upper_limit = std::acos(-1.0);

  const int status =
      gsl_integration_qags(&F, lower_limit, upper_limit, 1e-8, 1e-8, 1000,
                           workspace, &result, &error);

  gsl_integration_workspace_free(workspace);

  if (status != GSL_SUCCESS) {
    std::cerr << "Integration failed: " << gsl_strerror(status) << '\n';
    return 1;
  }

  std::cout << "Result: " << result << "\n";
  std::cout << "Estimated error: " << error << "\n";

  return 0;
}
