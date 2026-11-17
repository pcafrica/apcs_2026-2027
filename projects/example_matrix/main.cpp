// Build command:
// c++ -std=c++17 -Wall -Wextra -pedantic matrix.cpp main.cpp -o matrix_example

#include "matrix.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>

int main() {
  try {
    // Project 1 - Solar panel: rows follow y, columns follow x.
    Matrix<double> temperature(3, 4, 298.15);
    const std::size_t j = 1, i = 2;
    temperature(j, i) = 320.0;

    // Const access uses the const overload of operator().
    const Matrix<double> &field = temperature;
    const double mean =
        std::accumulate(field.begin(), field.end(), 0.0) / field.size();
    const double maximum = *std::max_element(field.begin(), field.end());

    std::cout << "Temperatures:\n";
    print(field);
    std::cout << "Selected temperature: " << field(j, i) << "\nMean: " << mean
              << "\nMaximum: " << maximum << '\n';

    // Copying creates independent storage.
    Matrix<double> copy = temperature;
    copy(j, i) = 310.0;
    if (temperature(j, i) != 320.0 || copy(j, i) != 310.0) {
      throw std::runtime_error("Matrix copies must have independent storage.");
    }

    // Project 2 - Bike sharing: observations on rows, features on columns.
    Matrix<double> features(3, 8);
    const std::size_t sample = 1, feat = 2;
    features(sample, feat) = 1.5;

    const bool valid =
        std::all_of(features.begin(), features.end(),
                    [](const double value) { return std::isfinite(value); });
    if (!valid) {
      throw std::runtime_error("Features must contain only finite values.");
    }

    std::cout << "Features:\n";
    print(features);

    // Project 3 - Neural network: output units on rows, input units on columns.
    Matrix<double> weights(8, 16);
    std::mt19937 engine(42);
    std::uniform_real_distribution<double> distribution(-0.1, 0.1);
    std::generate(weights.begin(), weights.end(),
                  [&engine, &distribution]() { return distribution(engine); });

    const std::size_t output = 0, input = 1;
    weights(output, input) = 0.25;
    std::cout << "Weights:\n";
    print(weights);

    // A separate small batch illustrates ReLU, not a full network forward pass.
    Matrix<double> activations(2, 3, -1.0);
    activations(0, 1) = 1.5;
    activations(1, 2) = 4.0;
    Matrix<std::uint8_t> mask(activations.n_rows(), activations.n_cols());

    // Unary transform builds the mask in an already-sized destination.
    std::transform(
        activations.begin(), activations.end(), mask.begin(),
        [](const double value) -> std::uint8_t { return value > 0.0; });

    // Binary transform applies the mask in place.
    std::transform(activations.begin(), activations.end(), mask.begin(),
                   activations.begin(),
                   [](const double value, const std::uint8_t active) {
                     return active != 0 ? value : 0.0;
                   });

    std::cout << "ReLU mask:\n";
    print(mask);
    std::cout << "ReLU activations:\n";
    print(activations);

    bool rejected = false;
    try {
      temperature(temperature.n_rows(), 0) = 1.0;
    } catch (const std::out_of_range &error) {
      rejected = true;
      std::cout << error.what() << '\n';
    }
    if (!rejected) {
      throw std::runtime_error("An invalid index must be rejected.");
    }

    rejected = false;
    try {
      const Matrix<double> invalid(0, 3);
    } catch (const std::invalid_argument &error) {
      rejected = true;
      std::cout << error.what() << '\n';
    }
    if (!rejected) {
      throw std::runtime_error("Zero dimensions must be rejected");
    }

    return 0;
  } catch (const std::exception &error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }
}
