#include "calculator.hpp"

#include <iostream>

int main() {
  char operation = '\0';
  double num1 = 0.0;
  double num2 = 0.0;
  double result = 0.0;

  std::cout << "Enter two numbers: ";
  if (!(std::cin >> num1 >> num2)) {
    std::cerr << "Error: Invalid numerical input." << std::endl;
    return 1;
  }

  std::cout << "Choose an operation (+, -, *, /): ";
  if (!(std::cin >> operation)) {
    std::cerr << "Error: Invalid operation." << std::endl;
    return 1;
  }

  switch (operation) {
  case '+':
    result = Calculator::add(num1, num2);
    break;
  case '-':
    result = Calculator::subtract(num1, num2);
    break;
  case '*':
    result = Calculator::multiply(num1, num2);
    break;
  case '/':
    if (num2 == 0.0) {
      std::cerr << "Error: Division by zero is not allowed." << std::endl;
      return 1;
    }
    result = Calculator::divide(num1, num2);
    break;
  default:
    std::cerr << "Error: Invalid operation." << std::endl;
    return 1;
  }

  std::cout << "Result: " << result << std::endl;

  return 0;
}
