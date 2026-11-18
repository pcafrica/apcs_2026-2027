#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

std::vector<std::pair<double, double>> generate_points(std::size_t n_points,
                                                       std::mt19937 &engine) {
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  std::vector<std::pair<double, double>> points(n_points);

  std::generate(points.begin(), points.end(), [&]() {
    const double x = distribution(engine);
    const double y = distribution(engine);
    return std::pair<double, double>{x, y};
  });

  return points;
}

// Each point must belong to the unit square.
double estimate_pi(const std::vector<std::pair<double, double>> &points) {
  if (points.empty()) {
    throw std::invalid_argument("At least one point is required");
  }

  const auto inside =
      std::count_if(points.begin(), points.end(), [](const auto &point) {
        const auto &[x, y] = point;
        return x * x + y * y <= 1.0;
      });

  return 4.0 * static_cast<double>(inside) / static_cast<double>(points.size());
}

int main() {
  const std::size_t n_points = 100'000;
  std::mt19937 engine{12345};
  const auto points = generate_points(n_points, engine);

  std::cout << "Points: " << points.size() << '\n';
  std::cout << "Estimated pi: " << std::setprecision(10) << estimate_pi(points)
            << '\n';
}
