#ifndef STATISTICS_HPP
#define STATISTICS_HPP

namespace stat {
// Precondition: numbers points to an array containing size > 0 elements.
double calculate_mean(const double *numbers, int size);
double calculate_standard_deviation(const double *numbers, int size);

// Bonus.
double calculate_median(const double *numbers, int size);
} // namespace stat

#endif // STATISTICS_HPP
