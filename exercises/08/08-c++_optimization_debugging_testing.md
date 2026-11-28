<!--
title: Exercise session 08
paginate: true

_class: titlepage
-->

# Exercise session 08
<br>

## Optimization, debugging, profiling, testing.
<br>

#### Advanced Programming for Computational Science<br>SISSA, UniTS, 2026-2027

###### Pasquale Claudio Africa

###### 01 Dec 2026

---

# Exercise 1: debugging a linked list

Work from [hints/ex1](hints/ex1/). The doubly linked list owns its nodes. Every node has `previous` and `next` pointers; a missing neighbor is represented by `nullptr`.

1. Fix the compilation errors, then inspect `find`, `print` and `erase` with GDB.
2. Correct the links when removing the head, a middle node or the tail.
3. Release erased nodes without destroying the remaining chain.
4. Check empty lists, missing values, `erase(nullptr)` and default-constructed nodes.
5. Run Memcheck and review whether an owning `List` or `Node` can safely be copied.

`find` returns a borrowed pointer. `append(Node*)` takes ownership of a non-null, unlinked heap node. `erase(Node*)` accepts a node of this list or `nullptr`.

---

# Exercise 2: testing numerical statistics

[hints/ex2](hints/ex2/) supplies `Statistics::mean` and the structure of five GoogleTest cases. Complete the indicated assertions in `tests/mean.cpp`.

Check empty input, one value, multiple values, mixed signs and a zero mean. Inspect the function to determine its behavior for empty input and select the appropriate assertion.

From `exercises/08`:

```bash
cmake -S hints/ex2 -B build-tests
cmake --build build-tests --parallel 4
(cd build-tests && ctest --output-on-failure)
```

GoogleTest supplies the assertions and test runner. CTest runs the discovered cases. Configuration fetches GoogleTest; prepare the dependency before an offline session.

---

<!--
_class: titlepage
-->

# Optional exercise<br>Matrix optimization

---

# Exercise 3: matrix optimization (1/3)

[hints/ex3](hints/ex3/) stores dense matrices in **column-major** order.

1. Implement `Matrix::transpose() const`: return a new matrix with exchanged dimensions, leaving the original unchanged.
2. Implement the ordinary matrix product `operator*`.
3. Implement `multiply_transposed`, transposing the first factor before multiplication. Include that preparation in its total cost.

The driver checks a rectangular example with known results, then compares both products before and after repeated timing. Its default benchmark size is 200.

Use the same inputs and compiler options. The transposed version's benefit depends on matrix sizes and hardware.

---

# Exercise 3: profiling and memory checks (2/3)

From your completed `hints/ex3` project, use the optimized Makefile with debug symbols:

```bash
make clean
make CPPFLAGS="-I. -DMATRIX_SIZE=20"
./ex3
valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./ex3
callgrind_annotate --auto=yes callgrind.out
valgrind --tool=memcheck --leak-check=full ./ex3
```

The smaller size keeps instrumented runs manageable. Inspect the baseline and transposed products. Request cache simulation with `--cache-sim=yes` when investigating cache events.

Measure elapsed time natively, with a larger dataset if needed. Timings printed under Valgrind include instrumentation overhead.

---

# Exercise 3: coverage (3/3)

Use the supplied `Makefile_coverage` with your completed matrix project:

```bash
make clean
make -f Makefile_coverage coverage CPPFLAGS="-I. -DMATRIX_SIZE=20"
```

Open `test_coverage/index.html`. Investigate unvisited lines and branches, and add relevant input cases when assessing the implementation.

This build uses `-O0 -g --coverage` and is separate from the optimized profiling build. Rebuild after changing build options.

The exercise's small cases check rectangular transpose, both matrix products and a matrix-vector product. The source structure follows the [course examples](https://github.com/cme212/course/tree/master/notes/lecture-01).
