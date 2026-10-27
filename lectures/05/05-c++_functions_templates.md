<!--
title: Lecture 05
paginate: true

_class: titlepage
-->

# Lecture 05
<br>

## Functions. Templates and generic programming in C++.
<br>

#### Advanced Programming for Computational Science<br>SISSA, UniTS, 2026-2027

###### Pasquale Claudio Africa

###### 27 Oct 2026

---

# Outline

<div class="columns">
<div>

#### 1. Functions and callable objects
- Function pointers
- Functors
- Lambdas and captures
- Function wrappers (`std::function`)

</div>
<div>

#### 2. Generic programming and templates
- Function and class templates
- Deduction and type requirements
- Compile-time values
- Code organization

</div>
</div>

**Appendices:** optional advanced techniques; STL previews for the next lecture.

---

<!--
_class: titlepage
-->

# Functions and callable objects

---

# Functions in C++

A **free function** is not a class member. A **member function** operates through a class interface. Both ordinary functions and function templates exist.

```cpp
double square(double x);                  // Ordinary declaration.
auto square(double x) -> double;          // Same return type.

auto cube(double x) { return x * x * x; } // Deduced return: double.
```

A function with a deduced return type must be **defined before a call that needs that type**. Function return-type deduction requires C++14 or later; all examples use **C++17**.

---

# Function signatures and overloading

For ordinary, non-template functions, overloads share a name but have different parameter-type lists. Member signatures can also differ in cv/ref qualifiers.

```cpp
int convert(int value);
double convert(double value);
// double convert(int value); // Error: return type alone is insufficient.

void inspect(int value);
void inspect(const int value); // Same function, not another overload.
```

The enclosing namespace/class matters. A **function name** and a **function signature** are different concepts; top-level `const` on a by-value parameter does not distinguish overloads.

---

# Free functions, state and side effects

Being a free function does **not** imply being stateless or pure.

```cpp
int calls = 0;
int next_call() { return ++calls; } // Reads and changes external state.

double square(double x) { return x * x; } // Pure for this example.
```

- A **pure function** has no observable side effects and its result depends only on its arguments.
- Free functions can use globals, local `static` variables, I/O or state reached through pointers/references.
- Choose a free function when class membership is unnecessary; choose purity separately.

---

# Returning values and references

Return a **value** for an independent result; return a **reference** when the caller should access an existing object.

```cpp
class Sample {
public:
    double& value() { return stored; }
    const double& value() const { return stored; }
private:
    double stored = 0.0;
};
```

References are common in accessors, `operator[]` and stream operators. Returning `std::ostream&` enables chaining: `std::cout << x << y`.

**Never return a reference to a local object that is destroyed on return.**

---

# Default function arguments

```cpp
double affine(double x, double scale = 1.0, double shift = 0.0) {
    return scale * x + shift;
}
```

```cpp
double a = affine(2.0);           // 2.0
double b = affine(2.0, 3.0);      // 6.0
double c = affine(2.0, 3.0, 1.0); // 7.0
```

For this ordinary parameter list, after the first default, later parameters must also have defaults. Put defaults in the declaration visible to callers; do not repeat them in its definition.

---

# Function overloading: a recap

```cpp
int fun(int i) { return i; }
double fun(const double& z) { return z; }
// double fun(double y); // Legal declaration, but would make fun(1.0) ambiguous.

auto x = fun(1);   // Calls fun(int).
auto y = fun(1.0); // Calls fun(const double&).
```

Overload resolution selects the best viable match. An ambiguity is diagnosed **at the call**, not merely because these two declarations coexist.

---

# What can be called?

- A **function** can be called directly or through a function pointer.
- A **functor** is a class object with `operator()`.
- A **lambda expression** creates a function object of an unnamed class type.
- `std::function` provides a common wrapper for compatible callables.
- A **member-function pointer** additionally needs an object; see Appendix A.

```cpp
double y = square(2.0); // Function call.
// The same call syntax, object(argument), also works with functors/lambdas.
```

These categories overlap: lambda objects and `std::function` objects are also function objects.

---

<!--
_class: titlepage
-->

# Function pointers

---

# Function pointers: pass behavior as input

```cpp
using f_ptr = double (*)(double);

double midpoint(double a, double b, f_ptr fun) {
    return (b - a) * fun((a + b) / 2.0);
}
```

```cpp
double square(double x) { return x * x; }

f_ptr f = &square; // The & is optional here.
double integral = midpoint(0.0, 2.0, f); // 2.0
```

`f_ptr` means “pointer to a function taking a `double` and returning a `double`”. The midpoint rule is an **approximation** of the integral.

---

# Dynamic function selection

```cpp
int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }

int choice = 1; // Could come from user input.
int (*operation)(int, int) = nullptr;

if (choice == 1)
    operation = add;
else if (choice == 2)
    operation = subtract;

if (operation != nullptr) {
    std::cout << operation(10, 5) << '\n';
}
```

Check a pointer that may be null before calling it. A function pointer does not carry a separate per-instance state.

---

<!--
_class: titlepage
-->

# Functors (function objects)

---

# Define a function object

```cpp
class Cube {
public:
    double operator()(double x) const { return x * x * x; }
};
```

```cpp
Cube cube;
double y = cube(3.0);   // 27.0
double z = Cube{}(2.0); // 8.0
```

The call invokes `operator()`. Declare it `const` when it does not modify the object's logical state. A callable returning a Boolean condition can serve as a **predicate**.

---

# Why functors? Store per-object state

```cpp
class Shift {
public:
    Shift(double offset) : offset(offset) {}
    double operator()(double x) const { return x + offset; }
private:
    double offset;
};
```

```cpp
Shift add_two(2.0), add_five(5.0);
double a = add_two(3.0);  // 5.0
double b = add_five(3.0); // 8.0
```

Each object has its own state. `operator() const` may **read** that state without modifying it.

---

# A functor can update its state

```cpp
class CountCalls {
public:
    int operator()() { return ++count; }
    int n_calls() const { return count; }
private:
    int count = 0;
};
```

```cpp
CountCalls ob1, obj2;
obj1(); obj1();
obj2();
// obj1.n_calls() == 2; obj2.n_calls() == 1.
```

Here `operator()` is non-const because it changes `count`. Copying a functor normally copies its state; it does not automatically share that state.

---

<!--
_class: titlepage
-->

# Lambda expressions

---

# A lambda creates a function object

```cpp
auto triple = [](double x) { return 3.0 * x; };
double y = triple(9.0); // 27.0

auto positive = [](double x) -> bool { return x > 0.0; };
```

- Syntax: `[captures](parameters) -> return_type { body }`.
- The return type can often be omitted and deduced from `return` statements.
- The compiler creates a **closure object** of a unique unnamed class type; `auto` stores that type.
- Using a lambda does not guarantee that the optimizer will inline its calls.

---

# Capture local values: an array example

```cpp
int values[] = {1, 5, 8, 12, 3, 7};
int threshold = 6;
auto above = [threshold](int x) { return x > threshold; };

int count = 0;
for (int x : values) { // Range-based for loop.
    if (above(x)) ++count;
}
// count == 3; above contains its own copy of threshold.
```

Changing `threshold` later does not change the already captured copy.

---

# Capture specification

| Capture | Meaning |
|---|---|
| `[]` | Capture nothing. |
| `[y]` / `[&y]` | Explicitly capture `y` by copy / reference. |
| `[=]` / `[&]` | Default to copy / reference for implicit captures. |
| `[=, &x]` | Copy implicit captures; explicitly capture `x` by reference. |
| `[this]` | Capture the pointer to the enclosing object. |
| `[*this]` | Capture a copy of that object (C++17). |

Capture defaults do **not** copy/reference every local variable indiscriminately. They govern eligible implicit captures needed by the lambda. Explicit captures are listed directly.

---

# Copies, references and mutable

```cpp
int count = 0;
auto by_copy = [count]() mutable { return ++count; };
auto by_reference = [&count]() { return ++count; };

int a = by_copy();      // 1: updates the closure's copy.
int b = by_copy();      // 2: original count is still 0.
int c = by_reference(); // 1: original count is now 1.
```

Without `mutable`, the copied `count` cannot be modified by this lambda. A reference capture lets the lambda modify the original object; `mutable` is not required for that.

---

# Capture lifetime: own a copy or borrow?

```cpp
auto make_scaler(double factor) {
    return [factor](double x) { return factor * x; };
}
```

```cpp
auto twice = make_scaler(2.0);
double y = twice(3.0); // 6.0: the captured copy still exists.
```

Returning `[&factor]` here would leave a dangling reference after `make_scaler` returns. Objects accessed through reference captures must remain alive whenever the lambda uses them.

`std::function` does not repair dangling reference captures.

---

# Capture this: access the original object

```cpp
class Product {
public:
    double compute(double a) {
        double values[] = {1.0, 2.0, 3.0};
        auto multiply = [this, a](double value) {
            this->x *= value * a;
        };
        for (double value : values) multiply(value);
        return x;
    }
private:
    double x = 1.0;
};
```

`Product p; p.compute(2.0);` returns `48.0` and changes `p`'s state. `[this]` does not extend `p`'s lifetime. `[*this]` instead captures a snapshot; modifying that snapshot requires `mutable`.

---

# Lambdas and function pointers

```cpp
using f_ptr = double (*)(double);
f_ptr square = [](double x) { return x * x; };

int factor = 3;
auto scaled = [factor](double x) { return factor * x; };
// f_ptr bad = scaled; // Error: this lambda has a capture.
```

A non-generic lambda with **no capture** can convert to a compatible function pointer. A capturing lambda carries state and cannot use this conversion.

A functor or lambda can later be passed directly to a **function template**.

---

<!--
_class: titlepage
-->

# Function wrappers

---

# std::function: one compatible call interface

```cpp
#include <functional>

int add(int a, int b) { return a + b; }

std::function<int(int, int)> task = add;
int result = task(2, 3); // 5

task = [](int a, int b) { return a * b; };
result = task(2, 3); // 6
```

`std::function<int(int, int)>` can hold different **copyable**, compatible callable targets. This is template syntax from the standard library; we explain templates in Part II.

---

# A closure type is not `std::function`

```cpp
auto lambda = [](double x) { return x * x; };
std::function<double(double)> wrapped = lambda;

double a = lambda(3.0);  // 9.0
double b = wrapped(3.0); // 9.0
```

`auto` deduces the lambda's own closure type. Initializing `wrapped` constructs a **separate wrapper** around a copy of that closure.

For a direct local call, `auto lambda = ...` is usually sufficient.

---

# An array of callable wrappers

```cpp
int add(int a, int b) { return a + b; }
struct Multiply {
    int operator()(int a, int b) const { return a * b; }
};

std::function<int(int, int)> tasks[] = {
    add, Multiply{}, [](int a, int b) { return a - b; }
};

for (const auto& task : tasks) {
    std::cout << task(3, 4) << '\n'; // 7, then 12, then -1.
}
```

Each element has the same wrapper type, despite the different target types. References in the loop avoid copying the wrappers and their state.

---

# Wrapper costs and lifetime

- A wrapper **owns its stored target**; copying it normally copies that target's state.
- Internally it uses **type erasure**: storage may be inline or dynamically allocated, depending on the target and implementation.
- Copying, storage and indirect calls can have a cost; measure when it matters.
- Referenced external objects still need valid lifetimes.
- A wrapper can be empty: check it before calling.

```cpp
std::function<double(double)> task;

if (task)
    std::cout << task(2.0) << '\n';
```

Flexibility is not automatic lifetime safety. No exception handling is needed in these examples: never invoke an empty wrapper.

---

<!--
_class: titlepage
-->

# Generic programming and templates

---

# Why generic programming?

```cpp
int maximum(int a, int b) { return a > b ? a : b; }
double maximum(double a, double b) { return a > b ? a : b; }
// Repeating the same algorithm for more types duplicates source code.
```

**Goal:** express an algorithm once for types supporting the required operations.

Templates provide compile-time parameterization. Benefits include reuse and type-specific optimization; costs include more complex diagnostics, compilation work and possible growth of generated code.

Type correctness does not guarantee correct logic, valid inputs or absence of overflow.

---

<!--
_class: titlepage
-->

# Function templates

---

# Define and instantiate a function template

```cpp
template <typename T> // Here, 'class T' would be the same as 'typename T'.
T add(T a, T b) {
    return a + b;
}
```

```cpp
int a = add<int>(5, 3);      // Explicit T = int.
double b = add(2.5, 3.7);    // Deduced T = double.
float c = add(1.0f, 2.0f);   // Deduced T = float.
```

The required expression `a + b` must be valid, and its result must be usable as a return value of type `T`. A template does not work with every conceivable type.

---

# Deduction versus explicit arguments

```cpp
// Using the add template from the previous slide:
// auto bad = add(3, 5.0); // Error: conflicting deductions for T.

double a = add<double>(3, 5.0); // T fixed explicitly; 3 converts to double.
auto b = add(3.0, 5.0);         // Both arguments deduce T = double.
```

Argument deduction is not a search for a convenient common numeric type. The destination variable's type does not resolve conflicting deductions for these parameters.

---

# Multiple parameters and return conversions

```cpp
template <typename T, typename U = double>
T multiply_and_add(T a, U b, T c) {
    return a * b + c;
}
```

```cpp
int a = multiply_and_add(5, 2.5, 3);      // T=int, U=double; returns 15.
int b = multiply_and_add<int>(2, 3, 1);   // T=int, U=int; returns 7.
float c = multiply_and_add<float, int>(2.5f, 3, 1.0f); // 8.5f
```

In the first two calls `U` is **deduced**, not taken from its default. The first expression gives `15.5`, then converts to return type `int`: templates do not prevent every narrowing conversion.

---

# When is a default template argument used?

```cpp
template <typename T = double>
T zero() { return T{}; }
```

```cpp
auto a = zero();      // T cannot be deduced: use default double.
auto b = zero<int>(); // Explicit T = int.
```

For a function template, a default fills an argument not specified explicitly or obtained by deduction. Here, a defaulted function-template parameter can precede a later deducible parameter.

Do not confuse default **function arguments** with default **template arguments**.

---

# Connect both halves: a generic midpoint rule

```cpp
template <typename Function>
double midpoint(double a, double b, Function fun) {
    return (b - a) * fun((a + b) / 2.0);
}
```

```cpp
double a = midpoint(0.0, 2.0, square); // Function pointer.
double b = midpoint(0.0, 2.0, Cube{}); // Functor.
double factor = 3.0;
double c = midpoint(0.0, 2.0,
                    [factor](double x) { return factor * x; }); // Lambda.
```

`Function` is deduced separately for each call. Each argument must support a call with a `double` and a result usable as a `double`. Here the callable is passed by value.

---

# Function pointer, wrapper or template?

| Interface                   | Useful when                                                             |
|-----------------------------|-------------------------------------------------------------------------|
| Function pointer            | A compatible function address is sufficient; no captured state.         |
| `std::function<Signature>`  | Different compatible target types need one common runtime wrapper type. |
| Function-template parameter | The caller's callable type can remain a compile-time parameter.         |
    
A template specialization may expose the concrete callable type to optimization, but **inlining is not guaranteed**. A wrapper can trade some overhead for a uniform interface.

All three still require valid callable targets and valid lifetimes.

---

<!--
_class: titlepage
-->

# Class templates

---

# A class template parameterizes a type

```cpp
template <typename T>
class Box {
public:
    Box() = default;
    Box(T value) : value(value) {}
    const T& get() const { return value; }
    void set(const T& new_value) { value = new_value; }
private:
    T value{};
};
```

```cpp
Box<int> integer(3);
Box<double> real(2.5);
```

`Box<int>` and `Box<double>` are different class types. Members are initialized; their operations impose requirements on `T`, such as construction and assignment.

---

# Class-template defaults

```cpp
template <typename T = double>
class Sample {
public:
    T value{};
};

Sample<> a;    // Default T = double.
Sample<int> b; // Explicit T = int.
```

In a primary class template, after a parameter receives a default, later parameters generally also need defaults (a trailing parameter pack is an exception).

Use the explicit `<>` here to emphasize the default template argument.

---

# `constexpr`: constant evaluation when required

```cpp
constexpr unsigned long long factorial(unsigned n) {
    return n <= 1 ? 1ULL : n * factorial(n - 1);
}
```

```cpp
constexpr auto five_factorial = factorial(5); // Required constant evaluation.
int values[factorial(4)]{}; // Constant array bound: 24 elements.

unsigned n = 5; // Could be read at runtime.
auto result = factorial(n); // A constexpr function can also run at runtime.
```

A `constexpr` variable needs a constant initializer. A `constexpr` function permits constant evaluation; it does not force every call to occur at compile time. Example domain: **0 <= n <= 20**, avoiding overflow of the return type.

---

# Values as template parameters

```cpp
template <typename T, int N>
class Array {
    static_assert(N > 0, "Array size must be positive");
public:
    T& operator[](int i) { return values[i]; }
    const T& operator[](int i) const { return values[i]; }
    int size() const { return N; }
private:
    T values[N]{};
};
```

```cpp
Array<double, 3> coordinates;
coordinates[0] = 1.5;
```

Here `N` is a compile-time integer value; indices must satisfy **0 <= i < N**. `static_assert` checks a condition during compilation. C++17 also permits other non-type parameter categories, such as pointers, which are not integral types.

---

# Alias templates

```cpp
// Using Array<T, N> from the previous slide:
template <int N>
using IntArray = Array<int, N>;

IntArray<5> values;
values[0] = 7;
```

An alias template gives a parameterized name to an existing type. `IntArray<5>` means `Array<int, 5>`; it does not create a separate class with different behavior.

---

# Template instantiation: some checks now, others later

- Templates are parsed when their definitions are read; syntax and non-dependent names are checked then.
- Substitution and many checks depending on a template parameter happen when a specialization is instantiated.
- Implicitly instantiating a class does not ordinarily instantiate the definitions of all unused member functions.

```cpp
// template <typename T>
// void broken() { missing_name(); } // Error even without a call: non-dependent name.

template <typename T>
T combine(T a, T b) { return a + b; } // Whether + works depends on T.
```

Templates delay **some** checks, not the whole compilation process.

---

<!--
_class: titlepage
-->

# Notes on code organization

---

# Definitions must be reachable for instantiation

With the usual implicit-instantiation model, callers need access to the template definition for the requested arguments.

```cpp
// twice.hpp
#ifndef TWICE_HPP
#define TWICE_HPP

template <typename T>
T twice(T value) { return value + value; }

#endif // TWICE_HPP
```

Include this header where `twice` is used. If only a declaration is visible and no needed specialization is supplied elsewhere, a call can end in an **undefined-reference linker error**.

---

# Three common organization patterns

1. **Header-only:** put template declarations and definitions in `module.hpp`.
2. **Included implementation header:** put declarations in `module.hpp`, definitions in `module.tpl.hpp` (another header file), and `#include` the latter at the end of the former, inside its guard.
3. **Explicit instantiation for a fixed set of types:** keep definitions in a `.cpp` and explicitly instantiate the supported specializations there.

Multiple units can instantiate the same template specialization; this is supported when the relevant definition rules are respected. Implementations commonly consolidate generated copies.

Exact code emission depends on compiler/linker optimizations. Repeated processing can still increase compilation time.

---

# Included implementation header

```cpp
// Alternative twice.hpp: declarations only.
#ifndef TWICE_HPP
#define TWICE_HPP
template <typename T> T twice(T value);

#include "twice.tpl.hpp"
#endif
```

```cpp
// twice.tpl.hpp: definitions. It is still a header file.
template <typename T>
T twice(T value) { return value + value; }
```

---

# Explicit instantiation: a fixed supported set

```cpp
// Alternative twice.hpp: declarations only.
#ifndef TWICE_HPP
#define TWICE_HPP
template <typename T> T twice(T value);
#endif
```

```cpp
// twice.cpp
#include "twice.hpp"
template <typename T>
T twice(T value) { return value + value; }

// Explicit instantiations.
template int twice<int>(int);
template double twice<double>(double);
```

Compile `twice.cpp` with the caller. With this layout, `int` and `double` are supplied; a call requiring an unsupplied `twice<char>` has no definition to link against.

---

# Explicit instantiation: what is generated?

```cpp
// After the complete Box<T> definition:
template class Box<int>;
template class Box<double>;
```

A class explicit-instantiation definition covers the applicable **non-template members defined at that point**; it does not automatically instantiate every possible member template or inherited operation.

With header-visible definitions, `extern template` declarations can suppress selected implicit instantiations in callers (although this is an optional build-time technique).

---

# Practical rules: callables

- Use a free function when membership is unnecessary; do not confuse that choice with purity.
- Use a functor or capture when behavior needs per-object state.
- Prefer explicit captures when they make ownership and lifetime easier to see.
- A closure's `auto` type is not `std::function`; use a wrapper when a uniform wrapper type is needed.
- Check possibly empty pointers/wrappers before calling them.
- Copying a callable usually copies its state; reference captures still borrow external objects.

---

# Practical rules: templates

- Make an operation generic when there is a concrete reuse need.
- State required operations and valid input domains; templates do not remove overflow or runtime preconditions.
- Distinguish argument deduction, explicit arguments, defaults and result conversions.
- Keep definitions visible for open-ended implicit instantiation; otherwise provide explicit specializations/instantiations deliberately.
- Use `constexpr` when constant evaluation is useful, without assuming every call runs at compile time.
- Prefer simple overloads/aliases before introducing specialization or metaprogramming machinery.

---

# Check your understanding

1. Why can two functor instances produce different results for the same argument?
2. How do `[x]` and `[&x]` differ after the original `x` changes?
3. Why does returning a lambda with a reference to a local variable fail?
4. Why can `add(2, 3.0)` fail even if its result is assigned to a `double`?
5. Why does hiding a template definition in a `.cpp` often cause an undefined reference?
6. How can the same midpoint algorithm accept a function, functor or capturing lambda?

---

<!--
_class: titlepage
-->

# Appendix A<br>Optional advanced techniques

---

# Member-function pointers: the class interface

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Square : public Shape {
public:
    Square(double side) : side(side) {}
    double area() const override { return side * side; }
private:
    double side;
};
```

This independent example reuses last lecture's runtime-polymorphism model. Next we store the address of an operation in this interface; calling it still requires an object.

---

# Member-function pointers: call through an object

```cpp
Square small(2.0), large(3.0);
Shape* shapes[] = {&small, &large};
double (Shape::*area_fun)() const = &Shape::area;

for (const Shape* shape : shapes) {
    double area = (shape->*area_fun)(); // 4.0, then 9.0.
    std::cout << area << '\n';
}
```

`->*` combines a pointer to an object with a member pointer; for an object/reference use `.*`.

Since `area()` is virtual, dispatch reaches `Square::area()`. These pointers borrow automatic objects: **do not delete them**.

---

# std::bind and function adapters

```cpp
#include <functional>

int add(int a, int b) { return a + b; }

std::function<int(int)> add5 = std::bind(add, 5, std::placeholders::_1);
int a = add5(3); // 8

auto add5_lambda = [](int value) { return add(5, value); };
int b = add5_lambda(3); // 8
```

`bind` fixes/rearranges arguments. For straightforward adaptations, a lambda usually makes the captured values and call easier to read. This adapter is optional, not a prerequisite for understanding callables.

---

# Wrapper and virtual dispatch: different mechanisms

```cpp
#include <functional>

auto compute_area = [](const Shape& shape) { return shape.area(); };
std::function<double(const Shape&)> wrapped = compute_area;

Square square(3.0);
double a = compute_area(square); // 9.0
double b = wrapped(square);      // 9.0
```

Both calls reach `Square::area()` because **area is virtual**. Wrapping a callable is separate from virtual dispatch in the Shape hierarchy.

---

# Function-template specialization

```cpp
#include <cctype>

template <typename T>
T maximum(T a, T b) { return a > b ? a : b; }

template <>
char maximum(char a, char b) {
    int upper_a = std::toupper(static_cast<unsigned char>(a));
    int upper_b = std::toupper(static_cast<unsigned char>(b));
    return upper_a > upper_b ? a : b;
}
```

`maximum('A', 'b')` returns `'b'` with the usual C-locale letter conversion. Conversion to `unsigned char` makes the `toupper` argument valid for arbitrary `char` values.

This is a **full** function specialization. For ordinary alternative parameter patterns, prefer an overload.

---

# Full class-template specialization

```cpp
template <typename T>
class Label {
public:
    const char* text() const { return "generic"; }
};

template <>
class Label<bool> { // Full specialization: all arguments are fixed.
public:
    const char* text() const { return "boolean"; }
};
```

`Label<int>` uses the primary template; `Label<bool>` uses the full specialization. A specialization defines its own class interface; it does not inherit the primary template's members.

---

# Partial specialization (only for class templates)

```cpp
template <typename T, int N>
class Array {
    static_assert(N > 0, "Positive size required");
public:
    T& operator[](int i) { return values[i]; }
private:
    T values[N]{};
};

template <typename T>
class Array<T, 1> { // Partial specialization: T remains a parameter.
public:
    T& operator[](int) { return value; } // Valid index: 0 only.
private:
    T value{};
};
```

This independent example specializes a subset of argument combinations. Function templates cannot be partially specialized; use **overloading** instead.

---

# Nested aliases and dependent type names

```cpp
template <typename T>
struct Wrapper {
    using value_type = T;
};

template <typename Container>
void make_value() {
    typename Container::value_type value{};
    (void)value;
}

make_value<Wrapper<int>>();
```

In C++17, a **qualified dependent name** not already known to denote a type generally needs `typename` in this context. Do not prefix every dependent type: `T` itself is already a known type parameter.

---

# Dependent bases and `this->`

```cpp
int selected() { return 1; }

template <typename T>
class Base {
public:
    int selected() const { return 2; }
};

template <typename T>
class Derived : public Base<T> {
public:
    int first() const { return selected(); }        // Global: returns 1.
    int second() const { return this->selected(); } // Base member: returns 2.
};
```

Unqualified lookup does not automatically search the dependent base `Base<T>`. `this->selected()` makes the lookup dependent. With these non-virtual methods, `Base<T>::selected()` also selects the base member.

---

# `auto`: extend deduction beyond fundamental types

```cpp
int value = 5;
auto copy = value;       // int: a separate value.
auto& reference = value; // int&: aliases value.
auto* pointer = &value;  // int*: points to value.
auto closure = [](int x) { return 2 * x; };
```

`auto x = 5` unambiguously deduces `int`; it does not choose among `int`, `long` and `short`.

Choose `auto` for readability, and choose `&`, `const` and pointer notation deliberately. In particular, plain `auto` does not preserve a reference returned by a function.

---

# Return-type deduction and `decltype`

```cpp
auto add_mixed(int a, double b) { return a + b; } // Deduces double.

auto add_trailing(int a, double b) -> decltype(a + b) {
    return a + b;
}

int value = 3;
decltype(value) copy = value;    // int
decltype((value)) alias = value; // int&
```

`decltype` describes the type of an expression without evaluating it, and can preserve reference information that plain `auto` deduction would discard. This prepares the following SFINAE example.

---

# Type traits and `if constexpr`

```cpp
#include <type_traits>

template <typename T>
void print_parity(T value) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << value % 2 << '\n';
    } else {
        std::cout << "Not an integral type\n";
    }
}
```

`print_parity(5)` prints `1`; `print_parity(2.5)` prints the message. In this function template, the discarded branch is not instantiated: the invalid `double % int` expression is avoided.

`if constexpr` and the `_v` trait forms used here require C++17. Discarded branches must still be syntactically valid.

---

# Variadic templates: consume a parameter pack

```cpp
template <typename T>
auto sum(T value) { return value; }

template <typename T, typename... Args>
auto sum(T first, Args... rest) {
    return first + sum(rest...);
}
```

```cpp
auto a = sum(1, 2, 3);      // 6
auto b = sum(1, 2.5);       // 3.5
auto c = sum(2.5, 1, 3.5);  // 7.0
```

`...` declares/expands a pack. The return is deduced at each step; forcing return type `T` would truncate some mixed-type results. This example requires at least one argument and compatible additions.

---

# C++17 fold expressions

```cpp
template <typename First, typename... Rest>
auto sum_fold(First first, Rest... rest) {
    return (first + ... + rest);
}
```

A fold can replace explicit recursive pack processing. This version accepts at least one argument; when `rest` is empty, the result is `first`.

Both recursion and folds still follow the participating types' `operator+` behavior and conversions. Floating-point addition order can affect rounding.

---

# Template template parameters

```cpp
// Using the previously defined Box<T>:
template <typename T, template <typename> class Storage = Box>
class Holder {
public:
    Holder(T value) : storage(value) {}
    const T& get() const { return storage.get(); }
private:
    Storage<T> storage;
};

Holder<double> a(2.5);
Holder<int, Box> b(3);
```

`Storage` receives a **template**, then is instantiated as `Storage<T>`. In contrast, `std::vector<std::complex<double>>` passes a complete **type** to `vector`; it does not require a template template parameter.

---

# Template metaprogramming: a small example

```cpp
template <int N>
struct Fibonacci {
    static_assert(N >= 0 && N <= 20, "Use 0 <= N <= 20 here");
    static constexpr int value = Fibonacci<N - 1>::value +
                                 Fibonacci<N - 2>::value;
};
template <> struct Fibonacci<0> { static constexpr int value = 0; };
template <> struct Fibonacci<1> { static constexpr int value = 1; };

constexpr int ten = Fibonacci<10>::value; // 55
```

This computes a constant through template instantiation and specialization. The deliberately small domain avoids negative recursion and integer overflow.

For simple numeric calculations, a `constexpr` function is often clearer; template metaprogramming is also useful for manipulating types. Neither technique is universally faster to compile.

---

# Traits and policy-based design

Traits describe properties of a type; policies provide interchangeable behavior selected through template arguments.

```cpp
struct ConsolePolicy {
    static void log(const std::string& message) {
        std::cout << message << '\n';
    }
};
struct SilentPolicy {
    static void log(const std::string&) {}
};

template <typename LogPolicy>
struct Processor {
    void run() { LogPolicy::log("Running task..."); }
};
```

`Processor<ConsolePolicy> a; a.run();` logs a message. `Processor<SilentPolicy> b; b.run();` stays silent. The policy choice is part of the class type.

---

# CRTP: static polymorphism

```cpp
template <typename Derived>
class Shape {
public:
    double area() const {
        return static_cast<const Derived*>(this)->area_impl();
    }
};

class Circle : public Shape<Circle> { // Curiously Recurring Template Pattern.
public:
    Circle(double radius) : radius(radius) {}
    double area_impl() const { return 3.14159265359 * radius * radius; }
private:
    double radius;
};
```

`Circle c(2.0); c.area();` selects the derived implementation through a compile-time pattern. There is no virtual dispatch here. `Shape<Circle>` and `Shape<Other>` would be **different base types**, not one heterogeneous runtime base interface.

---

# SFINAE: substitution failure is not an error

During function-template candidate formation, certain invalid substitutions in the **immediate context** remove a candidate instead of producing a hard error.

This can select overloads based on whether a type supports an operation. It does **not** make every error in a function body disappear.

---

# SFINAE: detect a callable print method

```cpp
#include <type_traits>
#include <utility>

template <typename T>
class has_print {
    template <typename U>
    static auto test(int) -> decltype(
        std::declval<U&>().print(), std::true_type{});

    template <typename U>
    static std::false_type test(...);
public:
    static constexpr bool value = decltype(test<T>(0))::value;
};
```

The first candidate is valid when a public `print()` call on a `U&` is well-formed. Otherwise the ellipsis fallback is selected. `test` is used only in unevaluated expressions, so its declarations need no function bodies.

---

# SFINAE: check the result

```cpp
class MyType {
public:
    void print() {}
};

std::cout << std::boolalpha;
std::cout << has_print<MyType>::value << '\n'; // true
std::cout << has_print<int>::value << '\n';    // false
```

The detected property is **a valid public zero-argument call on a non-const lvalue**, not merely the existence of any member named `print`. Overloads and qualifiers can affect the result.

---

<!--
_class: titlepage
-->

# Appendix B<br>STL previews for the next lecture

---

# Preview: containers, iterators and algorithms

The following slides are **next-lecture material**, not prerequisites for today's core examples.

- `std::vector<T>` is a dynamic sequence of values of type `T`.
- Iterators describe positions; algorithms commonly work on `[begin, end)` ranges.
- An output iterator can describe where generated values should be written.
- Algorithms can receive function pointers, functors or lambdas.

```cpp
#include <vector>

std::vector<int> values = {1, 2, 3};
// values.begin() and values.end() delimit the range.
```

Today we used ordinary arrays and loops to keep the focus on callables/templates.

---

# Preview: predefined functors

Include `<functional>`. These are class templates; `T` selects the operand/result type used by the operation.

| Functor | Operation | Arity |
|---|---|---|
| `plus<T>`, `minus<T>` | Addition, subtraction | Binary |
| `multiplies<T>`, `divides<T>` | Multiplication, division | Binary |
| `modulus<T>` | Remainder (`%`) | Binary |
| `negate<T>` | Unary minus | Unary |
| `equal_to<T>`, `not_equal_to<T>` | Equality comparisons | Binary |
| `greater<T>`, `less<T>`, ... | Ordering comparisons | Binary |
| `logical_and<T>`, `logical_or<T>` | Logical AND, OR | Binary |
| `logical_not<T>` | Logical NOT | Unary |

Use the `std::` prefix. The underlying operation must be valid for the chosen operand type.

---

# Preview: transforming and accumulating

Include `<vector>`, `<algorithm>`, `<iterator>`, `<numeric>` and `<functional>`.

```cpp
std::vector<int> input = {1, 2, 3, 4, 5};
std::vector<int> output;
std::transform(input.begin(), input.end(),
               std::back_inserter(output), std::negate<int>{});
int product = std::accumulate(input.begin(), input.end(),
                              1, std::multiplies<int>{});
```

`output` becomes `{-1, -2, -3, -4, -5}`; `product` is `120`.

`std::back_inserter(output)` is a **function returning an output iterator** that appends assigned values. It is not itself one of the predefined arithmetic functor classes.

The initial value and binary functor use consistent integer types here.

---

# Preview: sum a vector with a function template

```cpp
#include <vector>

template <typename T>
T vector_sum(const std::vector<T>& values) {
    T sum{};
    for (const T& value : values) {
        sum += value;
    }
    return sum;
}
```

Required operations include `T{}`, accumulation with **`operator+=`**, and returning a `T` value. `operator+` alone is insufficient.

`T{}` must represent the desired starting value: it gives zero for arithmetic types and an empty `std::string` for string concatenation (include `<string>`).

This makes the template's actual requirements explicit.

---

# Preview: a templated stack (LIFO)

```cpp
template <typename T>
class Stack {
public:
    void push(const T& value) { elements.push_back(value); }
    T pop() {
        if (elements.empty()) {
            std::cerr << "Stack is empty\n";
            std::exit(EXIT_FAILURE);
        }
        T top = elements.back();
        elements.pop_back();
        return top;
    }
private:
    std::vector<T> elements;
};
```

This simple implementation copies pushed/popped values and needs suitable `T` operations. Popping an empty stack prints an error and terminates; no explicit exception handling is introduced.

---

# Preview: algorithms can receive lambdas

```cpp
#include <algorithm>
#include <vector>

std::vector<int> values = {1, 5, 8, 12, 3, 7};
int threshold = 6;
auto count = std::count_if(values.begin(), values.end(),
                           [threshold](int value) { return value > threshold; });
// count == 3
```

This is the algorithm-based version of our earlier array loop. `auto` preserves the algorithm's count type. In the next lecture we explain the iterator range and algorithms.

---

# Preview: a vector of callable wrappers

```cpp
#include <functional>
#include <vector>

int add(int a, int b) { return a + b; }
struct Multiply {
    int operator()(int a, int b) const { return a * b; }
};

std::vector<std::function<int(int, int)>> tasks;
tasks.push_back(add);
tasks.push_back(Multiply{});
tasks.push_back([](int a, int b) { return a - b; });
for (const auto& task : tasks) std::cout << task(3, 4) << '\n';
```

This extends the earlier fixed array. All stored elements have the same wrapper type; targets differ. Output: `7`, `12`, `-1`.

---

<!--
_class: titlepage
-->

# :arrow_right: The Standard Template Library.
