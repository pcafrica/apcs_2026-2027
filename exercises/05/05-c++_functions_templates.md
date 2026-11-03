<!--
title: Exercise session 05
paginate: true

_class: titlepage
-->

# Exercise session 05
<br>

## Functions. Templates and generic programming in C++.
<br>

#### Advanced Programming for Computational Science<br>SISSA, UniTS, 2026-2027

###### Pasquale Claudio Africa

###### 04 Nov 2026

---

# Library tools used in this session

The focus is on callable objects and templates. The supplied code also uses these standard-library tools:

| Tool                             | Role                                                          | Header              |
|----------------------------------|---------------------------------------------------------------|---------------------|
| `std::vector<T>`                 | Store a sequence of values.                                   | `<vector>`          |
| `begin()`, `end()`               | Delimit a range; `end()` is past the last element.            | Container interface |
| `std::accumulate`                | Fold a range into one result, starting from an initial value. | `<numeric>`         |
| `std::sort` / `std::stable_sort` | Reorder a range using a strict ordering predicate.            | `<algorithm>`       |
| `std::copy_if`                   | Copy elements satisfying a predicate.                         | `<algorithm>`       |
| `std::back_inserter`             | Produce an output iterator that appends to a container.       | `<iterator>`        |

If these tools are new, use the supplied scaffold and this reference; their detailed study belongs to the STL lesson. Build examples with **C++17**; commands are in `README.md`.

---

# Exercise 1: lambda functions

Starting from `hints/ex1.cpp`, calculate the total cost of the supplied products.

1. Use `std::accumulate` with a lambda receiving a subtotal and a `const Product&`.
2. Start from **`0.0`** so the accumulator type is `double`.
3. Display each update and the final total using two decimal places.
4. Check the final result: **2799.95**. If checking floating-point values in code, use a tolerance rather than exact equality.

Prices use `double` to keep the exercise focused on lambdas. A production money representation would need an explicit rounding policy, for example using integer cents.

---

# Exercise 2: function pointers, functors and lambdas

Starting from `hints/ex2.cpp`, sort and filter the supplied collection of books.

1. Using `std::sort`, order books by year:
   - ascending, passing a free function as a **function pointer**;
   - descending, passing a **lambda**.
2. Using `std::stable_sort`, order books by the full author name in ascending lexicographic order, passing a **functor**. Preserve the preceding order within each equal-author group.
3. Use `std::copy_if` and `std::back_inserter` to fill `filtered_books` with books by a selected author. Implement the filtering predicate using a lambda that captures that author's name.
4. Display each result. For `"Jane Austen"`, expect exactly two books; also try an author with no matches.

An ordering predicate must return false for equivalent keys: use `<` or `>`, **not `<=` or `>=`**. The original product/book data and print helpers are already supplied.

---

# Exercise 3: Newton's method (1/4)

Starting from `hints/ex3/`, approximate **one root** of a scalar function with Newton's update:

$$
x_{k+1}=x_k-\frac{f(x_k)}{f'(x_k)}.
$$

Begin with $f:\mathbb R\to\mathbb R$, then extend the solver to suitable differentiable functions $f:\mathbb C\to\mathbb C$. In the complex case, `std::abs` measures the modulus.

Use the supplied `NewtonOptions`:

- residual tolerance $\varepsilon_f=10^{-12}$;
- scaled-step tolerance $\varepsilon_x=10^{-12}$;
- derivative threshold $\tau=10^{-14}$;
- at most **100 updates**.

Newton is a local method: a root or convergence is not guaranteed for every starting point.

---

# Exercise 3: check before dividing (2/4)

Implement these checks in this order:

1. Validate callable targets, a finite initial guess, finite positive tolerances, a finite non-negative derivative threshold and a positive update limit.
2. Compute $f(x_0)$; reject a non-finite value. **If $|f(x_0)|\le\varepsilon_f$, return success immediately**, even if $f'(x_0)=0$.
3. Before each division, evaluate $f'(x_k)$ and require a finite derivative with **$|f'(x_k)|>\tau$**. Otherwise return failure.
4. Compute $\delta_k=f(x_k)/f'(x_k)$ and $x_{k+1}=x_k-\delta_k$; reject non-finite updates. Then evaluate the new function value and residual.

The derivative threshold is an **absolute safeguard**, not a universal mathematical bound: function scaling matters. Setting $\tau=0$ permits every finite nonzero derivative.

---

# Exercise 3: residual and stopping rules (3/4)

After every finite update, compute

$$
r_{k+1}=|f(x_{k+1})|.
$$

- **Success:** $r_{k+1}\le\varepsilon_f$.
- **Stagnation:** if the residual is still too large but
  $$|\delta_k|\le\varepsilon_x(1+|x_{k+1}|),$$
  return failure: a small step alone is not proof of a root.
- **Other failures:** invalid input, non-finite values, rejected derivative, or exhausted iteration limit.

Return a supplied **`NewtonResult<T>`** with value, convergence flag, completed-update count, residual, last-step magnitude and a diagnostic message. Inspect the flag; do not encode failure in a `NaN` or compare with one.

---

# Exercise 3: wrappers, templates and files (4/4)

1. Complete the real-valued solver using `std::function<double(const double&)>` for both the function and its derivative.
2. Solve $f(x)=x^2-1$ from $x_0=0.5$: expect a root near **1**.
3. Make the solver a class template; solve $f(z)=z^2+1$ from $z_0=0.5+0.5i$: expect a root near **$i$**. Keep tolerances real-valued.
4. Try both organizations: definitions visible in a header, or declarations in a header with `.cpp` definitions and **explicit instantiations for `double` and `std::complex<double>`**.
5. Print the convergence flag, update count and final residual. Check an initially exact root, a zero derivative away from a root, iteration exhaustion and stagnation.

Complete solutions are in `solutions/ex3/` and `solutions/ex3/header_only/`. The same regression tests can run against both versions; compile them separately.

---

# Template arguments as policies · guided example

A **policy** supplies behavior selected through a template argument.

The `examples/` folder compares strings using two equality policies:

- `CaseCompare`: characters must match exactly;
- `NoCaseCompare`: compare their uppercase forms using safe `unsigned char` conversion;
- `compare<Policy>` uses the policy in a free function;
- `CompareString<Policy>` stores the policy in a function object.

These policies test **equality**, not the strict ordering required by `std::sort`. They require default construction; the stored policy must also be callable on a const object.

Case conversion follows the current C locale; this is not general Unicode case folding. Predict the four Boolean results before running the example.

---

# Exercise 4: template metaprogramming · optional

Use a class template to calculate factorial values at compile time.

1. Define `Factorial<N>` and the full specialization `Factorial<0>`.
2. Store the value as `unsigned long long`; restrict this exercise to **$0\le N\le20$** to avoid overflow.
3. Use `static_assert` to verify the constant-expression results:
   - $5!=120$;
   - $7!=5040$;
   - $10!=3628800$.
4. Print the results. The factorial values are compile-time constants; printing happens at runtime.

**Optional comparison:** implement the same calculation with a `constexpr` function and explain which version you find easier to read.
