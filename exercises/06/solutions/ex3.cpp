#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <vector>

class Statistic {
public:
  virtual double evaluate(const std::vector<double> &values) const = 0;
  virtual ~Statistic() = default;
};

class Mean : public Statistic {
public:
  double evaluate(const std::vector<double> &values) const override {
    if (values.empty()) {
      throw std::invalid_argument("The mean requires a nonempty dataset");
    }

    const double sum = std::accumulate(values.begin(), values.end(), 0.0);
    return sum / static_cast<double>(values.size());
  }
};

class Maximum : public Statistic {
public:
  double evaluate(const std::vector<double> &values) const override {
    if (values.empty()) {
      throw std::invalid_argument("The maximum requires a nonempty dataset");
    }

    return *std::max_element(values.begin(), values.end());
  }
};

int main() {
  const std::vector<double> values{1.0, 2.0, 3.0};

  std::unique_ptr<Statistic> original = std::make_unique<Mean>();
  std::unique_ptr<Statistic> transferred = std::move(original);
  std::unique_ptr<Statistic> maximum = std::make_unique<Maximum>();

  std::cout << "Original owner is empty: " << std::boolalpha << !original
            << '\n';
  std::cout << "Mean through unique_ptr: " << transferred->evaluate(values)
            << '\n';
  std::cout << "Maximum through unique_ptr: " << maximum->evaluate(values)
            << '\n';

  std::shared_ptr<Statistic> shared = std::make_shared<Mean>();
  auto another_owner = shared;

  std::cout << "Shared owners: " << shared.use_count() << '\n'; // 2
  std::cout << "Mean through shared_ptr: " << another_owner->evaluate(values)
            << '\n';

  another_owner.reset();
  std::cout << "Shared owners after reset: " << shared.use_count() << '\n'; // 1

  std::shared_ptr<Statistic> shared_maximum = std::make_shared<Maximum>();
  std::cout << "Maximum through shared_ptr: "
            << shared_maximum->evaluate(values) << '\n';
}
