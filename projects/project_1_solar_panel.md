# Project 1 — Solar-panel thermal model

## Purpose

Develop a C++ program for computing the steady-state temperature of a simplified photovoltaic panel. The work starts from a transparent standard-library implementation and grows into a reusable library with sparse solvers, tests, CMake, Python bindings, and comparison with SciPy.

No previous knowledge of partial differential equations, numerical grids, sparse matrices, third-party C++ libraries, or Python bindings is assumed. The model and the numerical formulas needed for the project are specified below.

---

## 1. Problem formulation

### 1.1 Physical setting and unknown

A temperature field varies continuously over a physical panel. Represent the panel by

\[
\Omega=[0,L_x]\times[0,L_y],
\qquad L_x=1.6\ \mathrm{m},
\qquad L_y=1.0\ \mathrm{m}.
\]

The unknown \(T(x,y)\), measured in kelvin, is the temperature at position \((x,y)\). The model is **steady**: temperature no longer changes with time, so there is no time derivative.

### 1.2 Governing equation

Use the thin-plate equation

\[
-K\Delta T+h(T-T_{\mathrm{amb}})=q(x,y)
\qquad \text{in }\Omega,
\]

with

\[
\Delta T=
\frac{\partial^2T}{\partial x^2}
+\frac{\partial^2T}{\partial y^2}.
\]

| Parameter            |                     Value | Meaning                                             |
|----------------------|--------------------------:|-----------------------------------------------------|
| \(K\)                |    \(0.45\ \mathrm{W/K}\) | in-plane conductivity multiplied by panel thickness |
| \(h\)                | \(10\ \mathrm{W/(m^2K)}\) | effective heat loss from the panel faces            |
| \(T_{\mathrm{amb}}\) |    \(298.15\ \mathrm{K}\) | ambient temperature                                 |

The Laplacian \(\Delta T\) measures local curvature of the temperature field. The term \(-K\Delta T\) models heat conduction within the panel, \(h(T-T_{\mathrm{amb}})\) models heat loss to the air, and \(q(x,y)\) is supplied heat per unit area. Every term has units of \(\mathrm{W/m^2}\).

The cooled frame imposes

\[
T(x,y)=T_{\mathrm{amb}}
\qquad \text{on }\partial\Omega.
\]

This is a **Dirichlet boundary condition**: temperatures on all four edges are prescribed data, not unknowns.

### 1.3 Heat-source models

The final program must provide these interchangeable sources:

1. Uniform heating:

   \[
   q_{\mathrm{uniform}}(x,y)=600\ \mathrm{W/m^2}.
   \]

2. A panel with a local hot spot:

   \[
   q_{\mathrm{hot}}(x,y)=
   \begin{cases}
   1200\ \mathrm{W/m^2}, & 0.60\le x\le1.00,\ 0.35\le y\le0.65,\\
   600\ \mathrm{W/m^2}, & \text{otherwise}.
   \end{cases}
   \]

For the hot-spot case, report maximum temperature and its position, mean temperature, and maximum temperature rise above ambient. Section 2.1 defines exactly which grid nodes enter these statistics.

### 1.4 Exact validation problem

Use the manufactured temperature

\[
T_*(x,y)=T_{\mathrm{amb}}+20
\sin\left(\frac{\pi x}{L_x}\right)
\sin\left(\frac{\pi y}{L_y}\right).
\]

It satisfies the boundary condition. Substitution into the equation gives the required source

\[
q_*(x,y)=
\left[
K\left(\frac{\pi^2}{L_x^2}+\frac{\pi^2}{L_y^2}\right)+h
\right]
20\sin\left(\frac{\pi x}{L_x}\right)
\sin\left(\frac{\pi y}{L_y}\right).
\]

A **manufactured solution** reverses the usual process: choose a smooth answer first and derive the source that makes it exact. This provides a known reference against which the numerical solution can be measured. No external dataset is needed for this project.

---

## 2. Methods needed to solve the problem

### 2.1 Numerical grid

A computer cannot store a continuous function at infinitely many positions. Replace the rectangle by the finite grid

\[
x_i=i\Delta x,\quad i=0,\ldots,N_x-1,
\qquad
y_j=j\Delta y,\quad j=0,\ldots,N_y-1,
\]

where

\[
\Delta x=\frac{L_x}{N_x-1},
\qquad
\Delta y=\frac{L_y}{N_y-1}.
\]

Here \(N_x\ge3\) is the number of grid nodes in the horizontal direction and \(N_y\ge3\) is the number in the vertical direction. The integer \(i\) selects an \(x\)-coordinate and \(j\) selects a \(y\)-coordinate. Therefore \(0\le i<N_x\) and \(0\le j<N_y\).

The number \(T_{i,j}\) approximates \(T(x_i,y_j)\). Nodes with \(i=0\), \(i=N_x-1\), \(j=0\), or \(j=N_y-1\) are boundary nodes and have the known value \(T_{\mathrm{amb}}\). All other nodes are interior unknowns. A \(5\times4\) grid therefore contains 20 nodes but only \((5-2)(4-2)=6\) unknown temperatures.

Store a full grid in a flat vector using

\[
\operatorname{index}(i,j)=jN_x+i.
\]

Compute reported temperatures over all \(N_xN_y\) grid nodes, including the prescribed boundary nodes:

\[
T_{\mathrm{mean}}=
\frac{1}{N_xN_y}
\sum_{j=0}^{N_y-1}\sum_{i=0}^{N_x-1}T_{i,j},
\qquad
\Delta T_{\max}=T_{\max}-T_{\mathrm{amb}}.
\]

If several nodes have the same maximum, report the first one in flat-storage order, that is, the smallest value of \(jN_x+i\).

When assembling the sparse system, number only interior nodes with

\[
p(i,j)=(j-1)(N_x-2)+(i-1).
\]

The grid notation used in this project is shown in the following figure. Blue nodes belong to the prescribed boundary, the orange node is the current interior unknown \(T_{i,j}\), and the four green nodes form its five-point stencil.

![Finite-difference grid with node indices, spacings, boundary nodes, and five-point stencil](assets/project_1_grid.svg)

### 2.2 Finite differences

Taylor expansion gives the centered approximations

\[
\frac{\partial^2T}{\partial x^2}(x_i,y_j)
\approx
\frac{T_{i+1,j}-2T_{i,j}+T_{i-1,j}}{\Delta x^2},
\]

\[
\frac{\partial^2T}{\partial y^2}(x_i,y_j)
\approx
\frac{T_{i,j+1}-2T_{i,j}+T_{i,j-1}}{\Delta y^2}.
\]

Substitution into the PDE produces one linear equation per interior node. Each equation involves the center and at most four neighbors, hence the name **five-point stencil**.

Let \(N=(N_x-2)(N_y-2)\) be the number of interior unknowns. The vector \(\mathbf t\) contains these \(N\) temperatures using the interior index \(p(i,j)\) defined above. The matrix \(A\) has \(N\) rows and \(N\) columns, and the right-hand-side vector \(\mathbf b\) has \(N\) entries. For the current interior node \((i,j)\), write \(p=p(i,j)\). Its diagonal coefficient is

\[
A_{pp}=2K\left(\frac1{\Delta x^2}+\frac1{\Delta y^2}\right)+h,
\]

and each neighbor that is also an interior node contributes

\[
A_{p,p(i-1,j)}=-\frac{K}{\Delta x^2}\quad\text{if }i>1,
\qquad
A_{p,p(i+1,j)}=-\frac{K}{\Delta x^2}\quad\text{if }i<N_x-2,
\]

\[
A_{p,p(i,j-1)}=-\frac{K}{\Delta y^2}\quad\text{if }j>1,
\qquad
A_{p,p(i,j+1)}=-\frac{K}{\Delta y^2}\quad\text{if }j<N_y-2.
\]

Only write one of these entries when the indicated neighbor is interior. Before accounting for boundary neighbors, set

\[
b_p=q(x_i,y_j)+hT_{\mathrm{amb}}.
\]

If a neighbor is on the boundary, move its known contribution to the right-hand side. For example, an \(x\)-boundary neighbor adds \(K T_{\mathrm{amb}}/\Delta x^2\) to \(b_p\).

### 2.3 Jacobi iteration

The first solver does not need a matrix. Initialize every entry of the old and new grids to \(T_{\mathrm{amb}}\). The boundary entries remain fixed for the entire iteration. During one sweep, update only the interior entries by rearranging the stencil as

\[
T_{i,j}^{\mathrm{new}}=
\frac{
q(x_i,y_j)+hT_{\mathrm{amb}}
+\frac{K}{\Delta x^2}(T_{i-1,j}^{\mathrm{old}}+T_{i+1,j}^{\mathrm{old}})
+\frac{K}{\Delta y^2}(T_{i,j-1}^{\mathrm{old}}+T_{i,j+1}^{\mathrm{old}})
}{
2K\left(\frac1{\Delta x^2}+\frac1{\Delta y^2}\right)+h
}.
\]

Use separate old and new vectors, so every right-hand-side value in one sweep comes from the old grid. After computing all new interior values, compute

\[
d=
\max_{\substack{1\le i\le N_x-2\\1\le j\le N_y-2}}
|T_{i,j}^{\mathrm{new}}-T_{i,j}^{\mathrm{old}}|.
\]

Use a strictly positive tolerance and a maximum iteration count of at least one. If \(d\) is at most the tolerance, the new grid is the converged solution. Otherwise swap the old and new grids and begin another sweep. If the iteration limit is reached first, return a non-converged result and report it clearly. This update is an accessible first stopping criterion, not an error bound on the exact temperature. Once the sparse system is assembled in Checkpoint 3, report the relative residual for each computed solution, including Jacobi when comparing solvers:

\[
\frac{\|\mathbf b-A\mathbf t\|_2}{\|\mathbf b\|_2}.
\]

For a vector \(\mathbf v=(v_1,\ldots,v_N)\), the Euclidean norm used here is \(\|\mathbf v\|_2=\sqrt{\sum_{k=1}^{N}v_k^2}\). The residual \(\mathbf r=\mathbf b-A\mathbf t\) measures how closely the computed temperature vector satisfies the assembled linear equations.

### 2.4 Sparse matrices and validation

A matrix is **sparse** when most entries are zero. Each row of this problem has at most five nonzero entries, so storing all the zeros of a dense matrix would waste memory and computation.

With \(N=(N_x-2)(N_y-2)\) interior nodes, the assembled matrix must contain

\[
\operatorname{nnz}
=N+2\left[(N_x-3)(N_y-2)+(N_x-2)(N_y-3)\right]
\]

stored nonzero entries. The first term counts diagonal entries; the two terms inside brackets count horizontal and vertical pairs of neighboring interior nodes, and the factor two counts both symmetric matrix positions. For the \(33\times21\) comparison grid, \(N=589\) and \(\operatorname{nnz}=2845\).

For the manufactured problem, let \(N=(N_x-2)(N_y-2)\) be the number of interior nodes and compute

\[
E_2=\sqrt{
\frac1N
\sum_{j=1}^{N_y-2}\sum_{i=1}^{N_x-2}
(T_{i,j}-T_*(x_i,y_j))^2
},
\qquad
E_\infty=
\max_{\substack{1\le i\le N_x-2\\1\le j\le N_y-2}}
|T_{i,j}-T_*(x_i,y_j)|.
\]

The two errors serve different purposes: \(E_2\) measures the typical error over the interior nodes, while \(E_\infty\) records the largest interior-node error. One prescribed validation grid is sufficient for the mandatory project; a multi-grid convergence study is optional.

### 2.5 Suggested C++ data structures

The mandatory physical computation uses `double`. A template may instead represent any useful varying type; it need not be a floating-point type. Course examples of CMake, pybind11, and vector or matrix classes may be adapted and reused; identify reused code in the README.

The following representation is sufficient; equivalent documented designs are acceptable:

- `Grid2D<T>` if useful: grid dimensions and a row-major `std::vector<T>`; keep physical spacings as `double` or in the thermal problem, and access nodes through `j * nx + i`;
- `ThermalProblem`: domain dimensions, `double` physical constants, and an owned heat-source object;
- `HeatSource`: abstract interface with `virtual double value(double x, double y) const = 0` and a virtual destructor;
- `UniformSource`, `HotSpotSource`, and `ManufacturedSource`: concrete implementations of `HeatSource`;
- `SolverOptions`: tolerance and maximum iteration count;
- `SolverResult`: temperature grid, convergence flag, iteration count, final update or residual, and summary statistics;
- Checkpoint 1 Jacobi storage: two full-grid vectors, one for old values and one for new values;
- Checkpoint 3 sparse storage: Eigen's sparse matrix type; students must not write their own sparse container.

Keep grid indexing in one function and validate indices in debug builds. The numerical solver should return a result object rather than relying on global variables. These names are examples; no template is required for the entire thermal model.

---

## 3. Checkpoint 1 — Standard-library baseline

Checkpoint 1 must use only C++ and the standard library. Do not include, download, import, or link any third-party dependency. Build it with one documented compiler command such as

```text
c++ -O2 -Wall -Wextra -pedantic src/*.cpp -Iinclude -o solar_panel
```

Proceed in this order:

1. store the complete grid in a flat `std::vector<double>` and centralize index and coordinate conversion;
2. initialize every boundary node to \(T_{\mathrm{amb}}\) and implement the uniform heat source;
3. verify the \(3\times3\) uniform-source problem, which has one interior unknown at \((x_1,y_1)=(0.8,0.5)\). Here \(\Delta x=0.8\), \(\Delta y=0.5\), and the exact discrete value is

   \[
   T_{1,1}=T_{\mathrm{amb}}+
   \frac{600}{
   2K(1/\Delta x^2+1/\Delta y^2)+h
   }
   \approx338.1333403\ \mathrm K.
   \]

4. implement Jacobi on a \(9\times7\) grid using tolerance \(10^{-10}\) and at most 100,000 sweeps; expose both values as command-line options;
5. write a CSV whose exact header is `x,y,temperature`. Write rows with \(j=0,\ldots,N_y-1\) as the outer loop and \(i=0,\ldots,N_x-1\) as the inner loop, and use enough decimal digits to recover a `double` value: `std::setprecision(std::numeric_limits<double>::max_digits10)`;
6. print iteration count, final update, maximum temperature, its coordinates, and mean temperature.

Testing in this checkpoint may use `assert` or an ordinary test executable that returns a nonzero exit code when a check fails. Four focused tests are sufficient: indexing and boundary values; one Jacobi update; convergence and invalid input; and the hand-computed \(3\times3\) case.

Required Checkpoint 1 deliverables are the source files, one build command, the small tests, and a short README explaining the grid and Jacobi algorithm.

### Software-quality requirements from Checkpoint 1 onward

- validate command-line arguments, file opening, grid sizes, physical parameters, tolerances, iteration limits, dimensions, and finiteness before using values;
- report user-input errors clearly and return a nonzero exit code;
- apply const-correctness: operations that do not change observable state must be `const`, and large read-only arguments should normally be passed by `const&`;
- use RAII and standard containers for every resource; do not use owning raw pointers or manual `new`/`delete`;
- prefer the Rule of Zero; if copy or move operations are defined, document why and test them;
- use `std::unique_ptr` for exclusive polymorphic ownership and give every polymorphic base class a virtual destructor;
- avoid memory leaks, dangling references, out-of-bounds access, and use-after-move errors;

Apply a requirement when its component is introduced: there is no polymorphic owner in Checkpoint 1. AI tools may be used under the common project rules; record their use and verification in `AI_USAGE.md`.

Handwritten test executables are sufficient at every checkpoint. A framework such as Catch2 or GoogleTest is optional. Maintain at most four focused test cases; one test case may verify several related properties.

---

## 4. Checkpoint 2 — Reusable C++ design

Refactor the working baseline without changing its numerical meaning. Proceed in this order:

1. Implement or adapt one template component that performs useful work with two actual types; test both uses and explain the avoided duplication in the README. For example, use `Grid2D<T>` for `double` temperatures and a `std::size_t` interior-node index map if the map is useful to your assembly. Alternatively, reuse a numerical solver component with `float` and `double` for a small precision comparison. These are examples, not extra mandatory grids or runs. The assessed physical results use `double`.
2. Define an abstract `HeatSource` with a virtual destructor and a const virtual `double value(double x, double y)` operation.
3. Implement `UniformSource`, `HotSpotSource`, and `ManufacturedSource` and select one at run time through `std::unique_ptr<HeatSource>`. This is the mandatory use of dynamic polymorphism; the source classes need not be templates.
4. Move Jacobi into a reusable library function or concrete class. A second solver interface is not required at this checkpoint.
5. Throw clear exceptions for invalid sizes, non-positive physical parameters, non-finite values, and invalid output paths. Represent failure to converge with the `SolverResult` convergence flag rather than returning an apparently valid solution.
6. Add simple CMake targets for the library, command-line application, and handwritten tests.

Templates generate type-specific code at compile time; dynamic polymorphism selects a heat-source implementation while the program is running. A template is justified by its two real uses, not by putting a type parameter on every class. Add the hot-spot and manufactured cases, then verify the manufactured solution on a \(17\times11\) grid using the Jacobi settings specified above. Require \(E_2<0.04\ \mathrm K\) and \(E_\infty<0.06\ \mathrm K\). These thresholds check the implementation against the manufactured solution; they are not adjustable fitting targets.

The command-line program must provide equivalent functionality to:

```text
solar_panel --nx 65 --ny 41 --source hotspot \
            --tolerance 1e-10 --max-iterations 100000 \
            --output temperature.csv
```

---

## 5. Checkpoint 3 — Libraries, Python, and comparison

After third-party libraries and linking have been introduced in the course, proceed in this order:

1. Assemble the interior system with `Eigen::SparseMatrix<double>` using triplets.
2. Solve it with `Eigen::SparseLU`, checking the factorization and solve status before using the result; no second sparse solver is required.
3. Register the existing handwritten tests with CTest. A testing framework remains optional.
4. Add a minimal pybind11 module exposing the source choice, grid dimensions, `solve`, the two-dimensional temperature values, and the relative residual. Return temperatures to Python as a NumPy array of shape `(ny, nx)` whose entry `[j, i]` is \(T_{i,j}\).
5. In one Python script, call the C++ binding for the manufactured problem on the \(33\times21\) grid. Assemble the same discrete system with SciPy and solve it with `scipy.sparse.linalg.spsolve`.

The final executable must report grid size, number of interior unknowns, matrix nonzeros, relative residual, maximum temperature, its coordinates, and mean temperature. For the manufactured case also report \(E_2\) and \(E_\infty\). If \(\mathbf t_{\mathrm{C++}}\) and \(\mathbf t_{\mathrm{SciPy}}\) contain the interior temperatures in the same order, define their relative solution difference as

\[
d_{\mathrm{rel}}=
\frac{\|\mathbf t_{\mathrm{C++}}-\mathbf t_{\mathrm{SciPy}}\|_2}
{\|\mathbf t_{\mathrm{SciPy}}\|_2}.
\]

Compare \(d_{\mathrm{rel}}\), the maximum absolute difference in kelvin, and the two relative residuals in a small table; investigate \(d_{\mathrm{rel}}>10^{-8}\). The absolute difference reveals discrepancies that a relative norm of temperatures near 298 K could make hard to see.

Run the hot-spot case once on the same grid and explain in the README where the maximum occurs. Record dependency versions and licenses in the README; a separate technical report is not required.

---

## Optional bonus extensions

Attempt at most one extension after all mandatory requirements work correctly:

- **Solution visualization.** Call the C++ solver through pybind11 and draw the hot-spot temperature map with [Matplotlib `pcolormesh`](https://matplotlib.org/stable/api/_as_gen/matplotlib.axes.Axes.pcolormesh.html), including axes in metres, a colour bar in kelvin, and a marker at the maximum.
- **Testing and profiling.** Move the four tests to [Catch2](https://catch2-temp.readthedocs.io/en/latest/) or [GoogleTest](https://google.github.io/googletest/primer.html), then profile one solve with [Valgrind Callgrind](https://valgrind.org/docs/manual/cl-manual.html) or benchmark it with [Google Benchmark](https://github.com/google/benchmark/blob/main/docs/user_guide.md). Make one justified optimization and recheck the solution.
- **Modern C++ or another library.** Adopt a newer standard or an additional library for a specific improvement, such as [`std::span`](https://en.cppreference.com/w/cpp/container/span) for non-owning grid views. Explain the concrete benefit and verify correctness. The justified use itself counts as bonus work.

---

## Final submission checklist

1. Source code and the required documentation, tests, scripts, and configuration files; no executables, build directories, virtual environments, or large result collections.
2. `README.md` with clean build, test, execution, and Python instructions.
3. CMake configuration, handwritten tests, binding, and comparison script.
4. Validation results for the manufactured problem.
5. Hot-spot results and physical interpretation.
6. Dependency versions, licenses, and attribution.
7. Contribution statement and `AI_USAGE.md` required by the common rules.
8. Name the archive using all group members' surnames, as required by `project_rules.md`.
