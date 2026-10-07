<!--
title: Exercise session 02
paginate: true

_class: titlepage
-->

# Exercise session 02
<br>

## Introduction to C++.
<br>

#### Advanced Programming for Computational Science<br>SISSA, UniTS, 2026-2027

###### Pasquale Claudio Africa

###### 07 Oct 2026

---

# Exercise 1: control structures

Write a C++ program `temperature_converter` that converts a given temperature from Celsius to Fahrenheit or vice versa.

- The program takes an input number from the user and the input unit as a string (or character).
- Use conditional statements to check if the user has provided a temperature in Fahrenheit or Celsius and print the corresponding output.
- The conversion formulas are:
  $$T_\text{Fahrenheit} = \frac{9}{5}T_\text{Celsius} + 32,$$
  
  $$T_\text{Celsius} = \frac{5}{9}(T_\text{Fahrenheit} - 32).$$

---

# Exercise 2: memory management

Create a C++ program that dynamically allocates memory for an array of integers.
- Allow the user to specify a positive size for the array and reject invalid input.
- Fill the array with random integers.
- Write a function called `find_min_max` to find the minimum and maximum values in the array. **Hint**: store the results in two variables passed by reference.

---

# Exercise 3: complete the missing statistics calculator

The `hints/ex3/` folder provides a partially implemented C++ program for calculating statistics for a set of numbers using dynamically allocated arrays and pointers. Your task is to fill in the missing parts.

- Ask the user how many numbers they want to enter. Entering `0` terminates the program.
- Dynamically allocate an array of `double` values with `new[]` and release it with `delete[]` after each calculation.
- Use the function prototypes in `statistics.hpp` to implement the mean and population standard deviation in `statistics.cpp`.
- The population standard deviation is $\sigma = \sqrt{\frac{1}{N}\sum_{i=1}^{N}(x_i - \bar{x})^2}.$
- **(Bonus)** Implement the median calculation: copy the input array, sort the copy with [`std::sort`](https://en.cppreference.com/cpp/algorithm/sort), calculate the median, and release the copied array.

---

# Exercise 4: code organization

Write a C++ program that simulates a simple calculator. Define functions for addition, subtraction, multiplication, and division. Allow the user to enter two numbers and choose an operation. Perform the chosen operation and display the result.

Here's a breakdown of the project structure:

- `calculator/`
    - `src/`
        - `main.cpp`
        - `calculator.cpp`
    - `include/`
        - `calculator.hpp`
    - `build/` (build artifacts, such as object files and executables)
    - `build.sh` (a shell script that compiles and properly links the code)
