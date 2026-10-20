#include <cmath>
#include <cstdlib>
#include <iostream>

void stop_with_error(const char *message) {
  std::cerr << "Error: " << message << '\n';
  std::exit(EXIT_FAILURE);
}

class ADExpression {
public:
  virtual ~ADExpression() = default;

  virtual double evaluate() const = 0;
  virtual double derivative() const = 0;
};

class Scalar : public ADExpression {
public:
  Scalar(double val, double der = 0.0) : val(val), der(der) {}

  double evaluate() const override { return val; }

  double derivative() const override { return der; }

private:
  const double val;
  const double der;
};

class Sum : public ADExpression {
public:
  Sum(const ADExpression &t1, const ADExpression &t2) : term1(t1), term2(t2) {}

  double evaluate() const override {
    return term1.evaluate() + term2.evaluate();
  }

  double derivative() const override {
    return term1.derivative() + term2.derivative();
  }

private:
  const ADExpression &term1;
  const ADExpression &term2;
};

class Difference : public ADExpression {
public:
  Difference(const ADExpression &t1, const ADExpression &t2)
      : term1(t1), term2(t2) {}

  double evaluate() const override {
    return term1.evaluate() - term2.evaluate();
  }

  double derivative() const override {
    return term1.derivative() - term2.derivative();
  }

private:
  const ADExpression &term1;
  const ADExpression &term2;
};

class Product : public ADExpression {
public:
  Product(const ADExpression &f1, const ADExpression &f2)
      : factor1(f1), factor2(f2) {}

  double evaluate() const override {
    return factor1.evaluate() * factor2.evaluate();
  }

  double derivative() const override {
    return (factor1.derivative() * factor2.evaluate()) +
           (factor1.evaluate() * factor2.derivative());
  }

private:
  const ADExpression &factor1;
  const ADExpression &factor2;
};

class Division : public ADExpression {
public:
  Division(const ADExpression &num, const ADExpression &den)
      : numerator(num), denominator(den) {}

  double evaluate() const override {
    const double den_val = denominator.evaluate();
    if (den_val == 0.0) {
      stop_with_error("Division by zero.");
    }
    return numerator.evaluate() / den_val;
  }

  double derivative() const override {
    const double num_val = numerator.evaluate();
    const double den_val = denominator.evaluate();
    if (den_val == 0.0) {
      stop_with_error("Division by zero.");
    }
    const double num_der = numerator.derivative();
    const double den_der = denominator.derivative();
    return (num_der * den_val - num_val * den_der) / (den_val * den_val);
  }

private:
  const ADExpression &numerator;
  const ADExpression &denominator;
};

class Power : public ADExpression {
public:
  Power(const ADExpression &b, int exp) : base(b), exponent(exp) {}

  double evaluate() const override {
    const double value = base.evaluate();
    if (value == 0.0 && exponent < 0) {
      stop_with_error("A negative power of zero is undefined.");
    }
    return std::pow(value, exponent);
  }

  double derivative() const override {
    // A zero exponent represents the constant function 1, also at x = 0.
    if (exponent == 0) {
      return 0.0;
    }
    const double value = base.evaluate();
    if (value == 0.0 && exponent < 0) {
      stop_with_error("A negative power of zero is undefined.");
    }
    return exponent * std::pow(value, static_cast<double>(exponent) - 1.0) *
           base.derivative();
  }

private:
  const ADExpression &base;
  const int exponent;
};

int main() {
  // Set x = 2.0 and its derivative (dx/dx) is 1.0.
  const Scalar x(2.0, 1.0);

  // Operands are named objects: references remain valid until main ends.
  // Define a polynomial: f(x) = 2x^3 - 3x^2 + 4x - 5.
  const Scalar two(2);
  const Scalar three(3);
  const Scalar four(4);
  const Scalar five(5.0);

  const Power x2(x, 2);
  const Power x3(x, 3);

  const Product term_4x(four, x);
  const Product term_2x3(two, x3);
  const Product term_3x2(three, x2);

  const Difference f1(term_2x3, term_3x2);
  const Sum f2(f1, term_4x);

  const Difference f(f2, five);

  // Compute the value and derivative of the polynomial at x.
  const double result = f.evaluate();
  const double derivative = f.derivative();

  // Print the result.
  std::cout << "f(" << x.evaluate() << ") = " << result << std::endl;
  std::cout << "f'(" << x.evaluate() << ") = " << derivative << std::endl;

  // Define the function 1 / x^2.
  const Scalar one(1.0);
  const Division g(one, x2);
  std::cout << "g(" << x.evaluate() << ") = " << g.evaluate() << std::endl;
  std::cout << "g'(" << x.evaluate() << ") = " << g.derivative() << std::endl;

  return 0;
}
