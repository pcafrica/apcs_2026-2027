<!--
title: Exercise session 04
paginate: true

_class: titlepage
-->

# Exercise session 04
<br>

## Inheritance and polymorphism in C++.
<br>

#### Advanced Programming for Computational Science<br>SISSA, UniTS, 2026-2027

###### Pasquale Claudio Africa

###### 21 Oct 2026

---

# Practical rules: inheritance and class design

- Use **public inheritance** for an **is-a** relationship: derived objects must respect the base interface and its promises.
- Use **composition** for owned parts; use **aggregation** for borrowed parts.
- Keep data members **private**; expose only the operations derived classes need.
- Initialize the **base class in the initializer list**, then members in declaration order.
- Remember the lifetime order: **base → members → constructor body**; destruction reverses it.
- With multiple inheritance, qualify ambiguous names with `obj.Base::member`. Use virtual inheritance when a diamond must share one base subobject.

---

# Practical rules: polymorphic interfaces

- Declare an operation `virtual` when its behavior must depend on the runtime object type.
- Write **`override`** in derived classes: the compiler checks the signature, including `const`.
- Pass polymorphic objects through **base pointers or references**; passing by base value causes **slicing**.
- Use a **pure virtual function** (`= 0`) to require an implementation in concrete derived classes.
- Give a base class a **public virtual destructor** when objects may be deleted through base pointers.

---

# Practical rules: ownership and safe use

- Decide explicitly whether each pointer or reference **owns or borrows** its object.
- Keep borrowed objects alive for as long as their pointers or references are used.
- Delete each dynamically allocated object **exactly once**, through its owning pointer; never delete an alias separately.
- Check pointers that may be **`nullptr`** before dereferencing them.
- For a factory returning an owning pointer, document that **the caller must delete the result**, including on early-return paths.
- Use pointer `dynamic_cast` for a checked downcast: failure returns **`nullptr`**; success creates an alias and does not transfer ownership.

---

# Exercise 1: inheritance and polymorphism for data analysis

Starting from the files in `hints/ex1`, create a C++ program that models different types of data sources and transformation objects.

1. Implement the `DataSource` hierarchy with `FileDataSource` and `ConsoleDataSource`.
2. Implement the `DataTransformer` hierarchy with `LinearScaler`, `LogTransformer` and `MinMaxScaler`.
3. Select a source and a transformation at runtime, using pointers to the base classes.

**The supplied `data_io.hpp` and `data_io.cpp` handle storage and input. You do not need to implement STL containers or file streams.** See `hints/ex1/README.md` for the completion order.

---

# Exercise 1 (1/5)

4. Complete the abstract class `DataSource`: add a public virtual destructor and a pure virtual `void read_data()` method. Its constructor, name, indexed data interface and display utilities are supplied.
5. Make `FileDataSource` and `ConsoleDataSource` publicly inherit from `DataSource`.
6. Implement the constructors: `FileDataSource` receives a filename and uses the source name `"File"`; `ConsoleDataSource` has a default constructor and uses the name `"Console"`.
7. Implement the derived destructors and mark overridden methods with `override`.

The input helpers open and close files automatically during reading. Empty derived destructors are sufficient; no manual stream management is required.

---

# Exercise 1 (2/5)

8. Study the supplied `read_data()` implementations: they delegate to the input helpers. File input reads whitespace-separated numbers; console input reads one non-empty line of numbers.
9. Use `n_elements()` and `operator[]` to inspect or modify data. `std::size_t` is the size/index type used by this supplied interface.
10. Use `display_info()` and `operator<<` to display the source and its values. File input can be read repeatedly; empty input, invalid numeric tokens and invalid indices print an error and terminate the program.
11. Try the supplied `data.txt`. Compile and run from the exercise root (`04`), following `README.md`.

---

# Exercise 1 (3/5)

12. Complete the abstract class `DataTransformer` with a constructor receiving `DataSource&`, a public virtual destructor and a pure virtual `void apply() const` method.
13. Store a non-owning reference to the source. The source must outlive the transformer. This is an association through a reference to the base class.
14. Implement `LinearScaler`, `LogTransformer` and `MinMaxScaler`, overriding `apply()`:
   - `LinearScaler`: multiply values by a scaling factor.
   - `LogTransformer`: apply the natural logarithm to positive values; replace non-positive values with zero.
   - `MinMaxScaler`: apply $(x-\min)/(\max-\min)$; map constant datasets to all zeros.
15. Reject empty datasets before transforming them. Use indexed loops; STL algorithms are not required.

---

# Exercise 1 (4/5)

16. Complete the menu in `main.cpp`: select `File` or `Console`, then `Linear`, `Log` or `MinMax`.
17. Allocate the derived objects and use them through `DataSource*` and `DataTransformer*`. Call `read_data()` and `apply()` through these base pointers.
18. Display the original and transformed data. Destroy the transformer before its referenced source. Clean up allocated objects also when the menu selection is invalid.
19. Check known results: `{1, 2, 3}` becomes `{0, 0.5, 1}` with min-max scaling; `{5, 5, 5}` becomes `{0, 0, 0}`. Test input errors in separate runs.

`apply() const` does not change the transformer itself; it can still modify the separate source object referred to by its reference member.

---

# Exercise 1 (5/5)

## Optional extensions

20. Ask the user for the filename instead of using `data.txt`.
21. Ask for the linear scaling factor instead of using `0.5`.
22. Add a `CSVDataSource` that imports a named field from `data.csv` (`x` or `y`). The optional `DataBuffer::read_csv(filename, field)` helper is supplied.
23. Add output to a text or CSV file. If overwriting the original CSV, preserve the header and the fields that were not transformed.

CSV and output extensions are optional. No exception handling is required: the supplied error helper prints a message and terminates the program.

---

# Exercise 2: automatic differentiation (1/3)

Implement a C++ framework for computing values and derivatives of expressions using polymorphism. Support addition, subtraction, multiplication, division and integer powers.

Test $f(x) = 2x^3 - 3x^2 + 4x - 5$ and $g(x) = 1/x^2$ at $x = 2$.

Expected results: $f(2)=7$, $f'(2)=16$, $g(2)=0.25$, $g'(2)=-0.25$.

---

# Exercise 2: automatic differentiation (2/3)

1. Define an abstract `ADExpression` class with a public virtual destructor and two pure virtual methods:
   - `double evaluate() const`: return the value of the expression.
   - `double derivative() const`: return its derivative with respect to the independent variable.
2. Implement `Scalar`, storing a value and its initial derivative. Use derivative `0` for constants and `1` for the independent variable.
3. Implement `Sum`, `Difference`, `Product`, `Division` and `Power` as derived classes, overriding both methods. `Power` receives an expression and a constant integer exponent.

---

# Exercise 2: automatic differentiation (3/3)

4. Store operands through `const ADExpression&`. Keep each operand alive for as long as an expression refers to it: create named intermediate objects, avoiding temporary operands such as `Sum(Scalar(1), Scalar(2))`.
5. For division, reject a zero denominator. For powers, reject zero raised to a negative exponent. Treat exponent zero as the constant function `1`, with derivative `0`, including at base zero.
6. Print both the value and derivative of each test expression.

Use an error message followed by program termination for invalid domains; `throw` and `catch` are not required.
