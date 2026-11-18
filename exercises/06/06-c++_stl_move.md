<!--
title: Exercise session 06
paginate: true

_class: titlepage
-->

# Exercise session 06
<br>

## The Standard Template Library, smart pointers and move semantics.
<br>

#### Advanced Programming for Computational Science<br>SISSA, UniTS, 2026-2027

###### Pasquale Claudio Africa

###### 18 Nov 2026



---

# Exercise 1: move semantics with `Vector` (1/3)

Start from [hints/ex1.cpp](hints/ex1.cpp): a numerical `Vector` with a private `double*` buffer, zero-initialized entries, deep copying and automatic cleanup.

1. Implement a **move constructor** transferring the buffer to the destination.
2. Implement **move assignment**, releasing any old destination buffer first.
3. Leave the source **valid and empty**: null buffer and size zero.

Use `n_elements()`, `operator[]` and the borrowed pointer from `storage()` to inspect the result. Indexing requires an in-range index.

The destination owns the transferred buffer; existing aliases still refer to it. These moves can be `noexcept`: pointer transfer and deletion of a `double` array do not throw.

---

# Exercise 1: ownership checks (2/3)

Check correctness before measuring performance:

- Copies have equal values in **independent buffers**; editing one does not change the other.
- Copy handles different sizes and self-assignment.
- A move preserves the buffer address and values, and empties the source. The destination may already own a buffer.
- The moved-from object can receive a new value by assignment.
- Copy/move of empty vectors and self-move are safe. For this implementation, self-move preserves the value.

Use checks that still run with `-DNDEBUG`. The supplied copy operations may throw on allocation failure; copy assignment leaves the destination unchanged if allocation fails.

---

# Exercise 1: assignment benchmark (3/3)

Compare **copy assignment** and **move assignment** for sizes **1024–2'097'152**, doubling the size at each step. Use **20 repetitions** per size.

1. Before each repetition, prepare an initialized source and an empty destination.
2. Use `std::chrono::steady_clock`; time only the assignment. Check the result after stopping the timer.
3. Write `measure_copy` and `measure_move`, each returning the mean time in microseconds as a `double`.
4. Print `elements copy_us move_us speedup`, with:

$$
\text{speedup} = \frac{\text{mean copy time}}{\text{mean move time}}.
$$

Compile with `-O2`; setup and destruction are outside the timer. Timings depend on the machine, compiler and workload.

---

# Exercise 2: sensor measurements (1/2)

Start from [hints/ex2.cpp](hints/ex2.cpp). File reading, row validation and grouping in a `std::map<int, std::vector<double>>` are supplied, together with:

```cpp
struct Summary {
    int sensor;
    std::size_t count;
    double mean;
};
```

1. Build a summary vector. Calculate means with `std::accumulate` starting from **`0.0`**; reject a non-finite sum.
2. Sort by **count descending**, then **sensor ID ascending** for equal counts.
3. Write `output.txt`, one row per sensor: `sensor_id count mean`, without a header. Check opening, writing and closing.

Keep the grouped input unchanged; compare IDs only when counts are equal.

---

# Exercise 2: sensor measurements (2/2)

For the supplied `input.txt` (run from `exercises/06`):

| Sensor | Measurements | Count | Mean |
|--------|--------------|-------|------|
| 1 | 1, 3 | 2 | 2 |
| 2 | 4, 6, 5 | 3 | 5 |
| 3 | -2, 2 | 2 | 0 |

Expected `output.txt`:

```text
2 3 5
1 2 2
3 2 0
```

Empty input produces empty output. Test malformed input and file errors; return a nonzero status on failure.

---

# Exercise 3: smart pointers and statistics (1/2)

Create an abstract `Statistic` class with a public virtual destructor and:

```cpp
virtual double evaluate(const std::vector<double>& values) const = 0;
```

1. Implement `Mean` and `Maximum`, overriding `evaluate`.
2. Borrow the input through the const reference; reject empty datasets with `std::invalid_argument`.
3. Construct each derived type using `std::unique_ptr<Statistic>`.
4. Transfer one owner with `std::move`. Check that the source pointer is empty and call `evaluate` through the destination.

For `{1,2,3}`, expect **mean 2** and **maximum 3**, also when called through a base pointer.

**Hints:** `std::accumulate` starts from `0.0`; dereference the result of `std::max_element` only for a nonempty range.

---

# Exercise 3: smart pointers and statistics (2/2)

5. Create `std::shared_ptr<Statistic>` instances for both derived types and call `evaluate` through them.
6. Copy one shared pointer. With exactly these two owners, check that `use_count()` is **2**.
7. Reset the copy. The remaining owner must still work, and its count must now be **1**.

The shared pointers own the **statistic object**. The input vector is borrowed only for the duration of `evaluate`.

Use `std::make_unique` / `std::make_shared`. Let the owners destroy the objects automatically through the virtual base destructor.

**Checks:** transferred unique ownership, both derived results through base pointers, shared counts 2 → 1, and rejection of an empty dataset.

---

<!--
_class: titlepage
-->

# Optional exercises

## Templates, deduplication and Monte Carlo

---

# (Optional) Exercise 4: `MyPair`

Implement a small class template with public `first` and `second` members, default/custom initialization, and `==`, `!=`, `<`.

- Default construction value-initializes both members.
- Equality compares both members.
- Lexicographic ordering compares first members; only when neither first member is less than the other does it compare second members.
- Test `int`, `double` and `std::string` with the operations they support.

**Checks:** `(1,9) < (2,0)`, but not the reverse; `(1,9) < (1,10)`; equal pairs are not less than themselves. Also compare `(1,"alpha")` and `(1,"beta")`.

Keep the implementation focused on copyable values with the required comparison operations.

---

# (Optional) Exercise 5: sampled sensor IDs (1/2)

1. Create `random_numbers` with **100** randomly sampled sensor IDs in **0–9**. Use a fixed seed for debugging.
2. Copy and sort it into `sorted_numbers`, keeping duplicates.
3. Build `sorted_unique_numbers`, sorted without duplicates.
4. Build `unsorted_unique_numbers`, keeping each ID's **first occurrence**, in its original order.
5. Print all four vectors. Keep `random_numbers` unchanged.

**Hints:** `std::sort` orders elements. `std::unique` compacts consecutive duplicates and returns the new logical end; use `erase` to remove the remaining tail.

For task 4, scan the original input and use an initially empty `std::unordered_set` to remember IDs already encountered.

---

# (Optional) Exercise 5: sampled sensor IDs (2/2)

Before using random data, test:

```text
Input:                 3 1 3 2 1
Sorted:                1 1 2 3 3
Sorted unique:         1 2 3
First occurrences:     3 1 2
```

Also check an empty input and an input whose entries are all equal.

**Alternative for task 3:** construct a `set` to order IDs and discard duplicates, then convert back to a vector:

```cpp
std::set<unsigned int> unique_ids(random_numbers.begin(),
                                  random_numbers.end());
std::vector<unsigned int> sorted_unique_numbers(unique_ids.begin(),
                                                unique_ids.end());
```


---

# (Optional) Exercise 6: Monte Carlo and algorithms (1/2)

Estimate $\pi$ using points uniformly sampled from the square $[0,1]^2$.

1. Set a positive sample size, initially **100,000**, and create a `std::mt19937` engine with seed **12345**.
2. Store the coordinates in a `std::vector<std::pair<double, double>>`.
3. Fill the vector using a uniform real distribution on $[0,1)$; `std::generate` can call a lambda once per element.
4. Use `std::count_if` to count points with $x^2+y^2\leq1$.
5. Print the sample size and the estimate:

$$
\widehat\pi = 4.0\,\frac{\text{points inside the quarter-circle}}{\text{total points}}.
$$

**Hint:** `point.first` and `point.second` are the two coordinates.

---

# (Optional) Exercise 6: Monte Carlo and algorithms (2/2)

Write an `estimate_pi` function taking the point vector by const reference.

- Reject an empty input with `std::invalid_argument`.
- Test `{(0,0), (1,0), (1,1), (0.5,0.5)}`: three points are inside, so the estimate is **3.0**.
- For generated points, the estimate must lie in **$[0,4]$**. A tiny sample need not approximate $\pi$ well.
- Try larger sample sizes. The typical statistical error decreases, but the error need not decrease at every trial.

Keep the seed fixed while debugging. Record the engine, seed and software environment: distributions may produce different sequences across libraries.

**Prerequisite:** the supplied points belong to the unit square. `end()` marks the end of the range and is not a point to inspect.
