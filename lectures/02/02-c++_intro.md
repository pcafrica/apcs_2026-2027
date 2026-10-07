<!--
title: Lecture 02
paginate: true

_class: titlepage
-->

# Lecture 02
<br>

## Introduction to C++. Built-in data types. Variables, pointers and references. Control structures. Functions.
<br>

#### Advanced Programming for Computational Science<br>SISSA, UniTS, 2026-2027

###### Pasquale Claudio Africa

###### 06 Oct 2026

---

# Why C++?

C++ is:

- Reasonably efficient in terms of CPU time and memory handling, being a **compiled language**.
- In high demand in industry.
- A (sometimes exceedingly) complex language: if you know C++ you will learn other languages quickly.
- A **strongly typed**$^1$ language: safer code, less ambiguous semantics, more efficient memory handling.
- Supports functional, object-oriented, and generic programming.
- Places strong emphasis on source compatibility.
- :evergreen_tree: It is <a href="https://medium.com/codex/what-are-the-greenest-programming-languages-e738774b1957" style="background-color: #69e781;">green</a>!

$^1$ Not everybody agrees on the definition of *strongly typed*.

---

# Outline

1. Structure of a basic C++ program
2. Fundamental types
3. Memory management: variables, pointers, references, arrays
4. Conditional statements and control structures
5. Functions and operators
6. User-defined types
7. Declarations and definitions
8. Code organization
9. The build toolchain in practice

---

<!--
_class: titlepage
-->

# Structure of a basic C++ program

---

# Structure of a basic C++ program

- C++ program structure includes a collection of functions.
- Every C++ program must contain one `main()` function, which serves as the entry point.
- Other functions can be defined as needed.
- Statements within functions are enclosed in curly braces `{}`.
- Statements are executed sequentially unless control structures (e.g., loops, conditionals) are used.

---

# Hello, world!

```cpp
#include <iostream>

int main() { // Or, more completely: int main(int argc, char** argv)
    std::cout << "Hello, world!" << std::endl;
    return 0;
}
```

- `#include <iostream>`: includes the Input/Output stream library.
- `int main()`: entry point of the program.
- `std::cout`: standard output stream.
- `<<`: stream insertion operator.
- `"Hello, world!"`: string to be printed.
- `<< std::endl`: inserts a newline and flushes the output stream.
- `return 0;`: indicates successful program execution.

---

# How to compile and run

To compile the program:

```bash
g++ hello_world.cpp -o hello_world
```

To run the compiled program:

```bash
./hello_world [arg1] [arg2] ... [argN]
```

To check exit code:
```bash
echo $?
```

---

# C++ as a strongly typed language

- C++ enforces strict type checking at compile time.
- Variables must be declared with a specific type.
- Type errors are detected and reported at compile time.
- This helps prevent runtime errors and enhances code reliability.

## Example

```cpp
int x = 5;
char ch = 'A';
float f = 3.14;

x = 1.6;        // Legal, but truncated to the 'int' 1.
f = "a string"; // Illegal.

unsigned int y{3.0}; // Uniform initialization (since C++11): illegal.
```

---

<!--
_class: titlepage
-->

# Fundamental types

---

# Fundamental types

| Data type                | `sizeof(...)` (bytes) |
|--------------------------|-----------------------|
| `bool`                   | 1                     |
| (`unsigned`) `char`      | 1                     |
| (`unsigned`) `short`     | 2                     |
| (`unsigned`) `int`       | 4                     |
| (`unsigned`) `long`      | 4 or 8                |
| (`unsigned`) `long long` | 8                     |
| `float`                  | 4                     |
| `double`                 | 8                     |
| `long double`            | 8, 12, or 16          |

---

# Integer numbers

- C++ provides several integer types with varying sizes.
- Common integer types include `int`, `short`, `long`, and `long long`.
- The range of values that can be stored depends on the type.

## Example

```cpp
int age = 30;

short population = 32000;

long long large_number = 123456789012345;
```

---

# Floating-point numbers

- C++ supports floating-point types for representing real numbers.
- Common floating-point types include `float`, `double`, and `long double`.
- These types can represent decimal fractions.

## Example

```cpp
float pi = 3.14;

double gravity = 9.81;
```

---

# Floating-point arithmetic

Floating-point arithmetic is a method for representing and performing operations on real numbers $\pm f \cdot b^e$ in a binary format (i.e., $b=2$).

- **Representation**: Floating-point numbers consist of three components: sign $s$ (0: positive, 1: negative), significand $f$, and exponent $e$.

- **Normalized numbers**: In normalized form, the most significant bit of the significand is always 1, allowing for a wide range of values to be represented efficiently.

- **IEEE 754 standard**: The most commonly adopted standard for floating-point arithmetic is the [IEEE 754 Standard for Floating-Point Arithmetic](https://ieeexplore.ieee.org/document/8766229). This standard specifies the formats, precision, rounding rules, and handling of special values like NaN (Not-a-Number) and infinity.

---

# Floating-point arithmetic limitations

```cpp
double epsilon = 1.0; // Machine epsilon.

while (1.0 + epsilon / 2.0 != 1.0) {
    epsilon /= 2.0;
}
```

```cpp
double a = 0.1, b = 0.2, c = 0.3;

if (a + b == c) { // Unsafe comparison.
    // This may not always be true due to precision limitations.
}
```

```cpp
double x = 1.0, y = 1.0 / 3.0; double sum = y + y + y;

if (std::abs(x - sum) < tolerance) { // Safer comparison.
    // Use tolerance to handle potential rounding errors.
}
```

---

# Characters and strings

- Characters are represented using the `char` type.
- Strings are sequences of characters and are represented using the `std::string` type.


## Example

```cpp
char comma = ',';

std::string name = "John";

std::string greeting = "Hello";

// Concatenate strings.
std::string message = greeting + comma + ' ' + name;
```

---

# Boolean types

- C++ has a built-in Boolean type called `bool`.
- It can have two values: `true` or `false`.
- Useful for conditional statements and logical operations.
- Numbers can be converted to Boolean.

## Example

```cpp
bool is_true = true;

bool is_false = false;

if (-1.5) { /* true */ }

if (0) { /* false */ }
```

---

# Initialization and aliases

- Initialization sets the initial value of a variable at the time of declaration.
- C++ supports various forms of initialization, including direct, copy, and list initialization.

## Example

```cpp
int x = 5; // Copy initialization.
int y(10); // Direct initialization.
int z{15}; // Uniform initialization (since C++11; preferred).
```

- Type aliases provide alternative names for existing types.

## Example
```cpp
using number = double;
using int_ptr = int*;
```

---

# `auto` and type conversions.

In many situations, the compiler can determine the correct type of an object using the initialization value.

```cpp
auto a{42};       // int.
auto b{12L};      // long.
auto c{5.0F};     // float.
auto d{10.0};     // double.
auto e{false};    // bool.
auto f{"string"}; // const char*.

// C++11.
auto fun1(const int i) -> int { return 2 * i; }

// C++14.
auto fun2(const int i) { return 2 * i; }
```

In reality, things are much more complex: see, for instance, [**Explicit type conversion**](https://en.cppreference.com/w/cpp/language/explicit_cast) and related pages.

---

<!--
_class: titlepage
-->

# Memory management: variables, pointers, arrays, references

---

# Automatic and dynamic storage

For now, we focus on two common cases:

- **Automatic storage**: a local variable is normally created when execution reaches its declaration and destroyed when execution leaves its block.
- **Dynamic storage**: an object is created with `new` and remains alive until it is destroyed with `delete`.

"Stack" and "heap" are common implementation terms. Local variables are typically stored on a stack, while dynamic storage is typically obtained from a heap. However, the C++ language formally specifies object lifetime and storage duration, not a particular stack or heap implementation.

---

# Variables

- Variables are named memory locations used to store data.
- They must be declared with a specific type before use.
- Variables can be modified and accessed in your program.

## Example

```cpp
int x = 5; // Declaration and initialization.
x = 10;    // Variable modification.

int y;  // Default-initialization; y has an indeterminate value here.
y = 20; // Assignment, not initialization.

const double a = 3.7; // 'const' means that the stored value cannot be modified.
a = 5; // Error!
```

---

# Pointers

- Pointers are variables that store memory addresses.
- They allow you to work with memory directly.
- Declared using `*` symbol.

## Example

```cpp
int number = 42;

int* pointer = &number; // Pointer to 'number'.
*pointer = 10; // Now also 'number' is 10.

int* dynamic_variable = new int; // A dynamic integer.
*dynamic_variable = 5;

// Deallocate it.
delete dynamic_variable;
dynamic_variable = nullptr;
```

---

# Arrays

- Arrays are collections of elements of the same type.
- Elements are accessed by their index (position).
- C++ provides the much safer `std::array<type, SIZE>`, `std::vector<type>` (to be covered in a later lecture).

## Example

```cpp
int numbers[5]; // Array declaration.
numbers[0] = 1; // Assigning values to elements.

int* dynamic_array = new int[5];

for (int i = 0; i < 5; ++i) {
    dynamic_array[i] = 2 * i;
}

delete[] dynamic_array;
```

---

# Pointers and arrays: common problems

```cpp
int* arr = new int[5]; // Dynamically allocate an integer array.

// Access and use the array beyond its allocated size.
for (int i = 0; i <= 5; ++i) {
    arr[i] = i;
}

// Forgetting to delete the dynamically allocated array causes a memory leak.
// delete[] arr;

// Attempt to access memory beyond the allocated array's bounds causes undefined behavior.
std::cout << arr[10] << std::endl;
```

---

# References

- References provide an alias for an existing variable.
- Declared using `&` symbol.
- Provide an alternative way to access a variable.

## Example

```cpp
int a = 10;

int& ref = a; // Reference to 'a'.
ref = 20; // Modifies 'a'.

int b = 10;
ref = b;
ref = 5; // What's now the value of 'a' and 'b'?
```

---

# Scope is not lifetime

- **Scope** determines where a name can be used.
- **Lifetime** determines when an object exists.
- **Storage duration** determines how long the object's storage lasts.

These concepts are related, but they are not interchangeable.

```cpp
int main() {
    int x = 2;

    { // Local scope.
        int local_value = 10; // Automatic storage.

        int* ptr = new int{20}; // The pointed-to int has dynamic storage.
        delete ptr;             // The pointed-to int is destroyed here. Lifetime ends here.

        ptr = nullptr; // Storage duration not expired yet,
                       // hence I can still use 'ptr'.
    } // local_value and ptr are destroyed here.
}
```

---

# Dynamic allocation: basic rules

- An object created with `new` must be destroyed with `delete`.
- An array created with `new[]` must be destroyed with `delete[]`.
- Failing to release dynamic memory causes a **memory leak**.
- Accessing an object after `delete` causes **undefined behavior**.
- A pointer and the object it points to are two different objects and may have different lifetimes.

```cpp
int* value = new int{42};
delete value;
value = nullptr;

int* values = new int[10]; values = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
delete[] values;
values = nullptr;
```

Later lectures will introduce safer abstractions that manage dynamic memory automatically.

---

<!--
_class: titlepage
-->

# Conditional statements and control structures

---

# `if` ... `else if` ... `else`

- Conditional statements allow you to execute different code based on conditions.
- In C++, we use `if`, `else if`, and `else` statements for conditional execution.

## Example

```cpp
int x = 10;

if (x > 5) {
    std::cout << "x is greater than 5." << std::endl;
} else if (x > 3) {
    std::cout << "x is greater than 3 but not greater than 5." << std::endl;
} else {
    std::cout << "x is not greater than 5." << std::endl;
}
```

---

# `switch` ... `case`

- The `switch` statement is a control flow structure alternative to using multiple `if` ... `else` statements based on the value of an expression.

## Example
```cpp
switch (expression) {
    case constant1:
        // Code to execute if expression == constant1.
        break;
    case constant2:
        // Code to execute if expression == constant2.
        break;
    // ... more cases ...
    default:
        // Code to execute if expression doesn't match any case.
}
```

---

# `for` loop

- A `for` loop is used to execute a block of code a specific number of times. It is often used when the number of iterations is known beforehand.

```cpp
for (initialization; condition; post-iteration operation) {
    // Code to execute in each iteration.
}
```

## Example

```cpp
for (int i = 0; i < 5; ++i) {
    std::cout << "Iteration: " << i << std::endl;
}
```

**NB**: `i` is usually named `counter`. A more general concept, called `iterator`, will provide a generalization for custom objects.

---

# `while` loop

- A `while` loop is used to repeat a block of code as long as the specified condition remains `true`. It is useful when the number of iterations is not predetermined.

```cpp
while (condition) {
    // Code to execute while the condition is true.
}
```

## Example
```cpp
int i = 0;
while (i < 5) {
  std::cout << "Iteration: " << i << std::endl;
  ++i;
}
```

**NB**: A `do { ... } while(...)` loop is similar to a `while` loop but guarantees that the block of code will be executed at least once, as the condition is checked after the loop body.

---


<!--
_class: titlepage
-->

# Functions and operators

---

# Functions
- Functions are blocks of code that perform a specific task.
- Functions are defined with a return type, name, and parameters.
- They can be called to execute their code.

## Example

```cpp
int add(int a, int b) {
    return a + b;
}

int result = add(3, 4); // Calling the 'add' function.
```

---

# `void`

- `void` is a data type that represents the absence of a specific type.
- It indicates that a function does not return any value or that a pointer does not have a defined type.
- Dangerous to use (especially in the case of `void*`).

## Example
```cpp
void greet() {
    std::cout << "Hello, world!" << std::endl;
}

void* generic_ptr;
int x = 10;

generic_ptr = &x; // Can point to any data type.
```

---

# Default arguments in functions

A function parameter can have a **default argument**, used when the caller does not provide a value for that parameter.

```cpp
void print_value(int value, int repetitions = 1) {
    for (int i = 0; i < repetitions; ++i) {
        std::cout << value << std::endl;
    }
}

print_value(42);    // repetitions is 1.
print_value(42, 3); // repetitions is 3.
```

- Parameters with default arguments must normally follow parameters without defaults.
- The caller can override a default argument by providing an explicit value.
- A default argument should be specified only once, usually in the function declaration.

---

# Pass by value vs. pointer vs. reference (1/2)

```cpp
void modify_by_copy(int x) {
    // Creates a copy of 'x' inside the function.
    x = 20; // Changes the copy 'x', not the original value.
}

void modify_by_ptr(int* ptr) {
    *ptr = 30; // Modifies the original value via the pointer.
}

void modify_by_ref(int& ref) {
    ref = 40; // Modifies the original value through the reference.
}
```

---

# Pass by value vs. pointer vs. reference (2/2)

```cpp
int value = 10;

modify_by_copy(value); // Pass by value.
std::cout << value << std::endl; // Output: 10.

modify_by_ptr(&value); // Pass by pointer
std::cout << value << std::endl; // Output: 30.

modify_by_ref(value); // Pass by reference
std::cout << value << std::endl; // Output: 40.
```

---

# Passing input: best practices

- Pass by value when the function should work on an independent copy.
- Pass by const reference when the function should read an existing value without copying it.
- Pass by non-const reference when the function must modify an existing value.
- Pass by pointer when `nullptr` has a meaningful interpretation or when working with C-style arrays.
- Clearly document who is responsible for releasing dynamically allocated memory.

---

# Return by value vs. pointer vs. reference (1/2)

```cpp
int get_copy() {
    return 42; // Return a copy of the value.
}

int* get_ptr(int& value) {
    return &value; // Points to an existing object.
}

int& get_ref(int& value) {
    return value; // Refers to an existing object.
}
```

---

# Return by value vs. pointer vs. reference (2/2)

```cpp
int result1 = get_copy(); // Return by value.

int value = 10;

int* result2 = get_ptr(value); // Return a pointer to 'value'.
*result2 = 15;                 // Modifies 'value'.

int& result3 = get_ref(value); // Return a reference to 'value'.
result3 = 20;                  // Modifies 'value'.
```

---

# Returning output: best practices

- Return by value by default: the caller receives an independent object.
- Return by reference only when referring to an existing object that will remain alive after the function returns.
- Return a pointer when the result may be `nullptr` or when a pointer is otherwise required.
- If a function returns dynamically allocated memory, clearly state who must call `delete`.
- Never return a pointer or reference to a local variable.

---

# A note on copy elision

When a function returns an object by value, the source code may appear to create an object and then copy it to the caller.

C++ compilers can often construct the returned object directly where it is needed, avoiding the copy. This mechanism is called **copy elision**.

- It is one reason why returning by value is often efficient.
- For simple fundamental types, such as `int`, copying is already inexpensive.

---

# `const` correctness (1/2)

```cpp
void print_value(const int x) {
    // x = 42; // Error: Cannot modify 'x'.
}

int get_copy() {
    const int x = 42;
    return x;
}

int result = get_copy();
result = 10; // Safe, it's a copy!

const int age = 30; // Immutable variable.
const int* ptr_to_const = &age; // Pointer to an integer which is constant.

ptr_to_const = &result; // Now pointing to another variable.
*ptr_to_const = 42; // Error: cannot modify pointed object.
```

**Question**: how to declare a constant pointer to a non-constant `int`?

---

# `const` correctness (2/2)

## Benefits
- Prevents unintended modifications: helps avoid accidental data modifications, enhancing code safety.
- Self-documenting code: makes code more self-documenting by indicating the intent of data usage.
- Expresses constraints that the compiler can check as part of the interface.

## Best practices
- Const correctness is a valuable practice for writing safe and maintainable C++ code.
- Use `const` to indicate read-only data and functions.
- Incorrectly using `const` can lead to compiler errors or unexpected behavior.

---

# Operators

- Operators are symbols used to perform operations on variables and values.
- Arithmetic operators: `+`, `-`, `*`, `/`, `%`
- Arithmetic and assignment operators: `+=`, `-=`, `*=`, `/=`, `%=`
- Comparison operators: `==`, `!=`, `<`, `>`, `<=`, `>=`, `<=>` (C++20)
- Logical operators: `&&`, `||`, `!`

## Example

```cpp
int x = 5, y = 3;
bool is_true = (x > y) && (x != 0); // Logical expression.
int z = (x > y) ? 2 : 1; // Ternary operator.

x += 2; // 7.
y *= 4; // 12.
z /= 2; // 1.
```

---

# Increment operators

1. **Pre-increment (`++var`)**:
   - Increases the variable's value before using it.
   - The updated value is immediately reflected. No temporary needed: **more efficient** (especially in the case of iterators).

2. **Post-increment (`var++`)**:
   - Uses the current value of the variable before incrementing.
   - The variable's value is increased after its current value is used.

```cpp
int a = 5;
int b = ++a; // Pre-increment. a is now 6, b is also 6.

int c = a++; // Post-increment. a is now 7, but c is 6.
```

---

# Function overloading

- Function overloading is a feature in C++ that allows you to define multiple functions with the same name but different parameters.
- The compiler selects the appropriate function based on the number or types of arguments during the function call.

```cpp
void print(int x) {
    std::cout << "Integer value: " << x << std::endl;
}

void print(double x) {
    std::cout << "Double value: " << x << std::endl;
}

print(3); // Calls the int version.
print(2.5); // Calls the double version.
```

---

<!--
_class: titlepage
-->

# User-defined types: `enum`, `union`, `struct`

---

# `enum`

- Enumerations (enums) allow you to define a set of named values.
- Enums provide a way to create user-defined data types.

## Example

```cpp
enum Color : unsigned int {
    Red = 0,
    Green,
    Blue
};

Color my_color = Green;
```

---

# `union`

- Unions allow you to define a type that can hold different data types.
- Only one member of a union can be accessed at a time.
- Useful for optimizing memory usage.

## Example

```cpp
union Duration {
    int seconds;
    short hours;
};

Duration d;
d.seconds = 259200;

short h = d.hours; // Contains garbage: undefined behavior.
```

---

# `struct`

- Structs (structures) allow you to group related data members into a single unit.
- Members can have different data types.
- Structs provide a way to create custom data structures.

## Example

```cpp
struct Point {
    int x;
    int y;
};

Point p;
p.x = 3;
p.y = 5;
```

---

# A note on `struct`s in C++

In C++, `struct` and `class` are closely related. Both can contain data and functions.

Their main difference is the default access level:

| Keyword  | Default member access |
|----------|-----------------------|
| `struct` | `public`              |
| `class`  | `private`             |

By convention, `struct` is often used to group simple data. "Plain Old Data" (POD) is a historical term with a precise but version-dependent definition. It should not be used simply to mean "a struct containing only data". More precise properties will be introduced when they are needed.

Classes and their additional features will be discussed in the next lecture.

---

# Simple data structs

A `struct` is commonly used to group related data. Its members can be initialized together and the entire object can be copied.


## Example

```cpp
struct Rectangle {
    double width;
    double height;
};

Rectangle r{10.0, 20.0}; // Initialize all members.
Rectangle s = r;         // Copy all members.
```

---

# Looking towards classes

- Object-oriented programming (OOP) is a programming paradigm that uses classes and objects.
- C++ is an object-oriented language that supports OOP principles.
- Classes are user-defined data types that encapsulate data and behavior.
- OOP promotes code reusability, modularity, and organization.

---

<!--
_class: titlepage
-->

# Declarations and definitions

---

# Declaration

- Declarations inform the compiler about the existence of variables or functions.
- They provide type information but do not allocate memory or provide implementation.

```cpp
extern int y; // Declaration of 'y'.

struct X; // Forward-declaration. What if I use both X inside Y and Y inside X?
struct Y { X var; };
struct X { /* ... */ }; // Full definition.
```

# Definition

- Definitions provide the actual implementation of variables or functions.
- They allocate memory for variables or specify the behavior of functions.

```cpp
int x = 5; // Definition of 'x'.
```

---

# Declaring functions
- Function declarations provide enough information for the compiler to use the function.
- They specify the return type, name, and parameter types.
- Function declarations are typically placed in header files.

## Example

```cpp
int add(int, int); // Declaration of 'add' function.
```

---

# Defining functions
- Function definitions specify the implementation of a function.
- They include the function's return type, name, parameters, and code block.
- They are typically placed in source files.

## Example

```cpp
int add(int a, int b) { // Definition of 'add' function.
    return a + b;
}
```

---

<!--
_class: titlepage
-->

# Code organization

---

# Modular programming

- Modular programming divides code into separate modules or units.
- Each module focuses on a specific task or functionality.
- Benefits:
  - Improved code organization and readability
  - Easier maintenance and debugging
  - Code reusability
  - Encapsulation of functionality

---

# Building blocks of C++ code modules

C++ code modules consist of:
- Header files (`.h` or `.hpp`) for declarations
- Source files (`.cpp`) for definitions
- Implementation files (`.cpp`) for non-template classes
- Header files contain function prototypes and class declarations.
- Source files contain function and class definitions.

---

# Header files

- Header files (`.h` or `.hpp`) contain **declarations**.
- They define the interface to a module or class.
- Header files are included in source files to access declarations.

```cpp
// my_module.hpp
int add(int a, int b); // Function prototype.
```

## Best practices
- Include only necessary headers to reduce compilation time.
- Use descriptive and unique names for header files.
- Document complex or non-obvious declarations.

---

# Source files

- Source files (`.cpp`) contain the **definitions** of functions and classes.
- They implement the functionality declared in header files.
- Source files include header files for access to declarations.

```cpp
// my_module.cpp
#include "my_module.hpp" // Include the corresponding header.

int add(int a, int b) {
    return a + b;
}
```

---

# The One Definition Rule (ODR)

A declaration introduces a name and can usually appear more than once:

```cpp
double add(double a, double b); // Declaration. Can be repeated in different compilation units.
```

A function or variable **must** normally have **exactly one definition** in the entire program:

```cpp
double add(double a, double b) { // Definition.
    return a + b;
}
```

- Function **declarations** are usually placed in **header files**.
- Function **definitions** are usually placed in **source files**.
- Identical type definitions, such as a `struct` placed in a header, may appear in multiple translation (compilation) units.
- Exceptions such as `inline` functions and templates will be discussed later.

---

# The need for header guards

- Header guards (or include guards) prevent multiple inclusions of the same header file.
- They ensure that a header file is included only once during compilation.
- Header guards are essential to avoid redefinition errors.

Without header guards, if a header file is included multiple times in a source file or in the same translation (compilation) unit, it can lead to multiple definition errors.

---

# How to implement header guards

- Place `#ifndef`, `#define`, and `#endif` or `#pragma once` directives in the header file.
- Use a unique identifier (usually based on the filename) as the guard symbol.

**Example (file `my_module.hpp`)**:

```cpp
#ifndef MY_MODULE_HPP__
#define MY_MODULE_HPP__

// ...

#endif // MY_MODULE_HPP__
```

Modern compilers also support:
```cpp
#pragma once

// ...
```

---

# Preventing header file inclusion issues

To avoid issues with header file inclusions:
- Use include guards or `#pragma once` to prevent multiple inclusions.
- Include necessary headers in your source files.
- Avoid circular dependencies (A includes B, and B includes A).
- Use forward declarations when possible to minimize dependencies.
- Follow a consistent naming convention for header guards.

---

# Using namespaces for organization

- Namespaces group related declarations to avoid naming collisions.
- They provide a way to organize code into logical units.
- Namespace members are accessed using the `::` operator.

```cpp
namespace Math {
    int add(int a, int b) {
        return a + b;
    }
}

int result1 = Math::add(3, 4); // Accessing a namespace member.

using namespace Math; // Useful, but dangerous due to possible name clashes.
int result2 = add(3, 4);
```

Anonymous (i.e. unnamed) namespaces are only accessible from the current compilation unit.

---

<!--
_class: titlepage
-->

# The build toolchain in practice

---

# Preprocessor and compiler

- The preprocessor (`cpp`) handles preprocessing directives.
- It includes headers, performs macro substitution, and removes comments.
- The compiler (`g++`, `clang++`) translates source code into object files.
- Preprocessor and compiler commands are combined when you run `g++` or `clang++`.

### Example (project with three files: `module.hpp`, `module.cpp`, `main.cpp`):

```bash
# Preprocessor.
g++ -E module.cpp -I/path/to/include/dir -o module_preprocessed.cpp
g++ -E main.cpp -I/path/to/include/dir -o main_preprocessed.cpp

# Compiler.
g++ -c module_preprocessed.cpp -o module.o
g++ -c main_preprocessed.cpp -o main.o
```

---

# Linker

- The linker (`ld`) combines object files and resolves external references.
- It creates an executable program from multiple object files.
- Linker errors occur if functions or variables are not defined.

## Example
```bash
g++ module.o main.o -o my_program
```

Link against an external library:
```
g++ module.o main.o -o my_program -lmy_lib -L/path/to/my/lib
```
In this example, the `-lmy_lib` flag is used to link against the library `libmy_lib.so`. The `-l` flag is followed by the library name without the `lib` prefix and without the file extension `.so` (dynamic) or `.a` (static).

---

# Preprocessor, compiler, linker: a simplified procedure

For small projects with few dependencies, the following command performs the preprocessing, compilation and linking phase:

```bash
g++ module1.cpp module2.cpp main.cpp -I/path/to/include/dir -o my_program
```

## :warning: Warning: different compilers may lead to different behavior!

Please keep in mind that different compilers can yield different/undefined behaviors and trigger distinct warnings or errors or print them in a less/more human-readable format.

For a demonstration, see this example on [Godbolt](https://godbolt.org/z/zn9d9vavK) comparing the output of GCC and Clang on the same code.

---

# Loader

- The loader loads the executable program into memory for execution.
- It allocates memory for the program's data and code sections.
- The operating system's loader handles this task.

## Example
```bash
./my_program
```

If a shared library is stored in a non-standard directory, its directory can temporarily be prepended to the loader search path before execution:
```bash
export LD_LIBRARY_PATH="/path/to/my/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
```

`LD_LIBRARY_PATH` is only one source used by the dynamic loader; it may also use standard directories, the system cache, and paths embedded in the executable such as `RPATH`.

--- 

<!--
_class: titlepage
-->

# :arrow_right: Classes and object-oriented programming
