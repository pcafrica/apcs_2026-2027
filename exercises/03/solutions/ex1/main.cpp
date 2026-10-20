#include "data_processor.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>

void check(bool condition, const char *message) {
  if (!condition) {
    std::cerr << "Test failed: " << message << '\n';
    std::exit(EXIT_FAILURE);
  }
}

void check_close(double actual, double expected, const char *message) {
  check(std::abs(actual - expected) < 1e-10, message);
}

int main() {
  const double input1[] = {2.43, -0.86, 7.19, 4.57, 1.68, 9.32, 5.75};
  const double input2[] = {0.73, -0.45, 0.12, 0.88, -0.67, 0.34, -0.92};

  check(DataProcessor::get_n_instances() == 0, "Initial instance count");
  const DataProcessor dp1(input1, 7);
  DataProcessor dp2 = dp1;
  DataProcessor dp3(input2, 7);
  dp3 = dp1;

  check(DataProcessor::get_n_instances() == 3, "Copy and assignment count");
  check(dp1.n_elements() == 7 && dp2.n_elements() == 7,
        "Constructor and copy sizes");
  check_close(dp3[1], input1[1], "Copy assignment values");
  dp2[0] = 100.0;
  check_close(dp1[0], input1[0], "Deep copy independence");
  dp3 = dp3;
  check_close(dp3[0], input1[0], "Self-assignment");

  check_close(dp1.min(), -0.86, "Minimum");
  check_close(dp1.max(), 9.32, "Maximum");
  check_close(dp1.compute_mean(), 4.297142857142857, "Mean");
  check_close(dp1.compute_std_dev(), 3.2230141805712345,
              "Population standard deviation");

  const DataProcessor dp4(input2, 7);
  {
    DataProcessor dp5 = dp1 + dp4;
    check(DataProcessor::get_n_instances() == 5, "Temporary instance count");
    check_close(dp5[0], 3.16, "Element-wise sum");
    const DataProcessor &readonly = dp5;
    check_close(readonly[3], 5.45, "Const indexing");
    dp5[3] = 0.0;
    check_close(readonly[3], 0.0, "Writable indexing");
  }
  check(DataProcessor::get_n_instances() == 4, "Destroyed instance count");

  const double short_input[] = {1.0, 2.0, 3.0};
  const double proportional[] = {2.0, 4.0, 6.0};
  {
    DataProcessor short_dp(short_input, 3);
    const DataProcessor proportional_dp(proportional, 3);
    dp3 = short_dp;
    check(dp3.n_elements() == 3, "Assignment between different sizes");
    check_close(dp3[2], 3.0, "Assignment between different sizes: values");
    check_close(compute_correlation(short_dp, proportional_dp), 1.0,
                "Pearson correlation");
  }

  std::cout << "dp1: " << dp1 << '\n';
  std::cout << "dp4: " << dp4 << '\n';
  std::cout << "Correlation between dp1 and dp4: "
            << compute_correlation(dp1, dp4) << '\n';
  std::cout << "All checks passed; active instances: "
            << DataProcessor::get_n_instances() << '\n';
}
