<!--
title: Exercise session 07
paginate: true

_class: titlepage
-->

# Exercise session 07
<br>

## Introduction to CMake.
<br>

#### Advanced Programming for Computational Science<br>SISSA, UniTS, 2026-2027

###### Pasquale Claudio Africa

###### 25 Nov 2026

---

# Today's session

**First half: guided examples (45 minutes)**

- Configure, build and organize a small library project.
- Select options, locate dependencies, install and test.

**Second half: exercises (45 minutes)**

1. Build an application using muParserX.
2. Express dependencies between two libraries and an application.

The appendix contains optional details for later exploration.

---

<!--
_class: titlepage
-->

# From source files to targets

---

# What does CMake do?

A hand-written Makefile describes how to build a project. This gives direct control, but include paths, dependencies and platform differences need maintenance.

CMake describes **targets** and their requirements, then generates files for a build tool such as Make or Ninja.

1. **Configure:** read `CMakeLists.txt`, process options and locate dependencies.
2. **Generate:** write build files for the selected tool.
3. **Build:** invoke that tool to compile and link.
4. **Test / install:** run tests or copy selected files, when requested.

The first two steps normally happen in the same `cmake` invocation.

---

# Why CMake?

- It supports several build tools and IDEs across platforms.
- Targets can carry include paths, language requirements and dependencies.
- Many scientific libraries provide CMake support, including Eigen, LAPACK and Trilinos.

Other choices include [GNU Autotools](https://www.gnu.org/software/automake/manual/), [Meson](https://mesonbuild.com/) and [Bazel](https://bazel.build/).

Package managers such as [Conan](https://conan.io/) and [vcpkg](https://vcpkg.io/) can supply dependencies. **Finding an installed package and installing it are separate tasks.**

---

# Our guided project

Open [example_math](example_math/CMakeLists.txt). It contains a library and an application:

```text
example_math/
├── CMakeLists.txt
├── math/
│   ├── CMakeLists.txt
│   ├── math.hpp
│   └── math.cpp
└── apps/
    ├── CMakeLists.txt
    └── main.cpp
```

The application calls `add(5, 3)` and `multiply(4, 6)`: expect **8** and **24**.

---

# Configure and build

Run these commands from `exercises/07/example_math`:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

cmake --build build --parallel 4

./build/apps/math_app
```

`-S` selects the source directory; `-B` selects a separate build directory.

CMake stores configuration choices in `build/CMakeCache.txt` and generates build files there. Re-run the build after changing a source file.

These examples require **CMake 3.15 or newer**, including `cmake --install`.

---

# Organize the project

The root `CMakeLists.txt` defines the project and visits its subdirectories:

```cmake
cmake_minimum_required(VERSION 3.15)

project(MathExample VERSION 1.0 LANGUAGES CXX)

include(CTest)

add_subdirectory(math)
add_subdirectory(apps)
```

Each subdirectory contains its own `CMakeLists.txt`. Process `math` first so that `apps` can use its target.

Command names are case insensitive; use lowercase consistently.

---

# Define the library target

In `math/CMakeLists.txt`:

```cmake
add_library(math SHARED math.cpp math.hpp)

target_include_directories(math PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(math PUBLIC cxx_std_17)
target_compile_options(math PRIVATE -Wall -Wextra -Wpedantic)
```

- `SHARED`: build a shared library; `STATIC`: build an archive linked into a program at link time.
- Listing headers helps IDEs; compiler dependency information tracks included headers. Listing them does **not** publish include paths or install them.
- `MODULE` is another library type, intended for explicitly loaded plugins.

---

# Use the library target

In `apps/CMakeLists.txt`:

```cmake
add_executable(math_app main.cpp)

target_link_libraries(math_app PRIVATE math)
target_compile_options(math_app PRIVATE -Wall -Wextra -Wpedantic)
```

Linking the target also propagates its public include directory and C++ language requirement. No manual `-I`, `-L` or library filename is needed.

| Visibility | Used by this target | Passed to its users |
|------------|---------------------|---------------------|
| `PRIVATE` | Yes | No |
| `PUBLIC` | Yes | Yes |
| `INTERFACE` | No | Yes |

---

# Variables and paths

A variable can hold a string or a list of semicolon-separated items:

```cmake
set(MATH_SOURCES math.cpp math.hpp)

add_library(math SHARED ${MATH_SOURCES})
```

This is an **alternative** to the earlier `add_library` call.

- `CMAKE_SOURCE_DIR` / `CMAKE_BINARY_DIR`: top-level source / build directory.
- `CMAKE_CURRENT_SOURCE_DIR` / `CMAKE_CURRENT_BINARY_DIR`: source / build directory for the currently processed subdirectory.

Keep `main.cpp` in the executable target, not in the library's source list.

---

<!--
_class: titlepage
-->

# Configuration choices and dependencies

---

# Cache variables persist

Cache variables store user choices across configuration runs:

```cmake
set(DATA_LABEL "temperature" CACHE STRING "Name of the measured field")
option(WITH_ARRAY "Use std::array for the sample buffer" OFF)
```

```bash
cmake -S . -B build -DDATA_LABEL=pressure -DWITH_ARRAY=ON
cmake -S . -B build -L
```

`set(... CACHE ...)` supplies a default when no cache entry exists. Reconfiguring without `-D` preserves the previous choice; use `-DWITH_ARRAY=OFF` to change it back.

Use a fresh build directory when you want a separate configuration.

---

# Conditions and compile definitions

For an executable target named `sample_app`:

```cmake
option(WITH_ARRAY "Use std::array for the sample buffer" OFF)

if(WITH_ARRAY)
    target_compile_definitions(sample_app PRIVATE USE_ARRAY=1)
endif()
```

`if(WITH_ARRAY)` tests the variable's value. For text comparisons, write an explicit comparison:

```cmake
if(DATA_LABEL STREQUAL "temperature")
    message(STATUS "Processing temperature samples")
endif()
```

---

# Select the C++ branch

The definition controls preprocessing when `sample_app` is compiled:

```cpp
#include <array>
#include <vector>

#ifdef USE_ARRAY
std::array<double, 100> samples{};
#else
std::vector<double> samples(100);
#endif
```

Both branches create 100 zero-initialized samples. `#ifdef` checks whether the macro exists: even `USE_ARRAY=0` would select the first branch.

The CMake option takes effect when the program is **rebuilt**, not at runtime.

---

# Find an installed package

`find_package` can use a **find module** or a package's own **config file**:

- Modules: `FindName.cmake`, from CMake or `CMAKE_MODULE_PATH`.
- Config files: e.g. `BoostConfig.cmake`, supplied by an installation.

For a target `simulation` that uses Boost filesystem and graph:

```cmake
find_package(Boost 1.92 CONFIG REQUIRED COMPONENTS filesystem graph)

target_link_libraries(simulation PRIVATE Boost::filesystem Boost::graph)
```

These **imported targets** describe installed libraries, including their usage requirements. `REQUIRED` stops configuration if the requested package is unavailable.

`find_package` does not download or install the package.

---

# Help CMake find dependencies

Pass installation prefixes, not source directories:

```bash
cmake -S . -B build \
    -DCMAKE_PREFIX_PATH="$HOME/local/boost;$HOME/local/muparserx"
```

A package-specific hint is also possible:

```bash
cmake -S . -B build -DBoost_ROOT="$HOME/local/boost"
```

Some packages export variables rather than imported targets. Use their documented interface; we will encounter this with muParserX in Exercise 1.

When diagnosing a lookup problem, check which package configuration and library CMake actually selected.

---

# Messages and build diagnostics

```cmake
message(STATUS "Project version: ${PROJECT_VERSION}")
message(WARNING "Check the selected data directory")
message(FATAL_ERROR "Required input data is missing")
```

`STATUS` reports information, `WARNING` allows configuration to continue, and `FATAL_ERROR` stops it. Choose the relevant message for your situation.

To inspect compiler and linker commands:

```bash
cmake --build build --verbose
```

For configuration details, use `cmake -S . -B build --trace-source=CMakeLists.txt`.

---

<!--
_class: titlepage
-->

# Install and test

---

# Select files to install

In `math/CMakeLists.txt`:

```cmake
install(TARGETS math
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(FILES math.hpp DESTINATION include)
```

In `apps/CMakeLists.txt`:

```cmake
install(TARGETS math_app RUNTIME DESTINATION bin)
```

Installation copies the selected outputs and headers into the chosen prefix; it does not rebuild them.

---

# Install in your own directory

From `example_math`, configure a user-owned prefix, build, then install:

```bash
cmake -S . -B build -DCMAKE_INSTALL_PREFIX="$HOME/local/math-example"
cmake --build build --parallel 4
cmake --install build

LD_LIBRARY_PATH="$HOME/local/math-example/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" \
    "$HOME/local/math-example/bin/math_app"
```

The prefix contains `bin/math_app`, `lib/libmath.so` and `include/math.hpp`.

The loader must also find the installed shared library. Here we supply its directory with `LD_LIBRARY_PATH`, as in Lecture 08.

---

# Run a test with CTest

The root file uses `include(CTest)`, which enables testing by default. In `apps/CMakeLists.txt`:

```cmake
if(BUILD_TESTING)
    add_test(NAME arithmetic COMMAND math_app)
endif()
```

The supplied application prints its results and returns a nonzero status if either calculation is wrong. CTest uses the exit status to determine success.

```bash
cmake --build build --parallel 4
(cd build && ctest --output-on-failure)
```

Testing is a separate step from building and installing.

---

<!--
_class: titlepage
-->

# Your turn: exercises

---

# Exercise 1: use an external library

**20 minutes.** Use the supplied [ex1.cpp](solutions/ex1/ex1.cpp), without looking at the solution's `CMakeLists.txt` yet.

1. Install [muParserX 4.0.12](https://github.com/beltoforion/muparserx/releases/tag/v4.0.12) in a directory you own (see the next slide).
2. Create a CMake project with an executable `ex1` and locate muParserX using `find_package(muparserx 4.0.12 EXACT CONFIG REQUIRED)`.
3. Apply its `muparserx_INCLUDE_DIRS` and `muparserx_LIBRARIES` to the executable with `PRIVATE` visibility.
4. Configure with `CMAKE_PREFIX_PATH`, build and run. Check the reported library path.

**Expected:** 10 evaluations of `a + c * strlen(s) - f`; the first is `0.9`, the last `109.8+108.9i` (formatting may vary).

---

# Exercise 1: dependency setup

Download and extract the release linked on the previous slide. From its source directory:

```bash
cmake -S . -B build \
    -DCMAKE_POLICY_VERSION_MINIMUM=3.10 \
    -DBUILD_EXAMPLES=OFF -DBUILD_SHARED_LIBS=ON \
    -DCMAKE_INSTALL_PREFIX="$HOME/local/muparserx"

cmake --build build --parallel 4
cmake --install build
```

The policy option allows this release's older CMake setup to configure with CMake 4.

**Hint:** the source includes `<mpParser.h>`. The package's include variable points directly to the directory containing that header. Its config file exports variables, not an imported target.

---

# Exercise 2: transitive dependencies

**20 minutes.** Use these sources without opening the solution's `CMakeLists.txt`:

- [math_lib.hpp](solutions/ex2/math_lib.hpp), [math_lib.cpp](solutions/ex2/math_lib.cpp): multiplication.
- [graphics_lib.hpp](solutions/ex2/graphics_lib.hpp), [graphics_lib.cpp](solutions/ex2/graphics_lib.cpp): rectangle output.
- [ex2.cpp](solutions/ex2/ex2.cpp): the application.

Write a CMake project with static targets `math` and `graphics`, and executable `ex2`. Express `graphics` → `math` and `ex2` → `graphics` using `target_link_libraries` and explicit visibility.

**Check:** link the application only to `graphics`; CMake must also arrange linking with `math`. Expected rectangle area: **0.75**.

Use the final **5 minutes** to compare results and inspect any failed builds.

---

<!--
_class: titlepage
-->

# Appendix: optional details

---

# Build generators: Make and Ninja

Use separate build directories when changing the generator:

```bash
cmake -S . -B build-make -G "Unix Makefiles"
cmake --build build-make --parallel 4

cmake -S . -B build-ninja -G Ninja
cmake --build build-ninja --parallel 4
```

The selected build tool must be installed. Ninja is designed for generated build files and can reduce build overhead in large projects.

CMake generates dependency information for both; actual build times depend on the project and environment.

---

# Environment variables and configuration

```cmake
message(STATUS "PATH: $ENV{PATH}")

set(ENV{DATA_ROOT} "/path/to/data")
```

This changes the environment of the current CMake process and processes it starts. It does not change the parent shell or a later, separately invoked build.

Prefer cache variables for persistent configuration choices.

For first-time compiler selection, use a **fresh** build directory:

```bash
cmake -S . -B build-clang -DCMAKE_CXX_COMPILER=clang++
```

---

# More configuration choices

For Make and Ninja with a single configuration, select `Debug`, `Release`, `RelWithDebInfo` or `MinSizeRel` at configuration time:

```bash
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
```

- Use `target_compile_options` for target-specific flags instead of changing global `CMAKE_CXX_FLAGS`.
- `CMAKE_LIBRARY_OUTPUT_DIRECTORY` can change where built libraries are placed; it does not control installation.
- `target_compile_features(math PUBLIC cxx_std_17)` requires at least C++17 and propagates that requirement to users.
- Alternatively, set `CXX_STANDARD 17` together with `CXX_STANDARD_REQUIRED ON`; these target properties alone do not propagate a language requirement to users.

---

# Generate a version header (1/2)

Template `print_version.hpp.in`:

```cpp
#ifndef PRINT_VERSION_HPP
#define PRINT_VERSION_HPP

#include <iostream>

inline void print_version() {
    std::cout << "Version number: @MY_PROJECT_VERSION@\n";
}

#endif
```

The version becomes part of a string literal; `inline` permits including this function definition in several translation units.

---

# Generate a version header (2/2)

For an existing target `math_app`, generate the header and make its build directory visible:

```cmake
set(MY_PROJECT_VERSION "2.1.5")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/print_version.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/print_version.hpp"
    @ONLY)

target_include_directories(math_app PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
```

`@ONLY` replaces `@VAR@` placeholders, preserving `${VAR}` text. The generated header goes in the build directory.

See also [`#cmakedefine`](https://cmake.org/cmake/help/latest/command/configure_file.html) for conditional definitions.

---

# Check whether a feature compiles

For a probe source that includes `main`:

```cmake
try_compile(HAVE_FEATURE
    "${CMAKE_CURRENT_BINARY_DIR}/probe-build"
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/probe.cpp")

if(HAVE_FEATURE)
    message(STATUS "Feature available")
endif()
```

The result is **`TRUE` or `FALSE`**, not a numeric exit status. This runs during configuration, unlike tests registered with CTest.

[`try_run`](https://cmake.org/cmake/help/latest/command/try_run.html) also executes the compiled probe.

---

# Building third-party source projects

Follow each project's build instructions; not every library uses CMake.

For a Doxygen source checkout (with its dependencies installed):

```bash
cmake -S . -B build -DCMAKE_INSTALL_PREFIX="$HOME/local/doxygen"
cmake --build build --parallel 4
cmake --install build
```

For [GSL 2.8](https://ftp.gnu.org/gnu/gsl/gsl-2.8.tar.gz), from its extracted source directory:

```bash
./configure --prefix="$HOME/local/gsl" --enable-shared --disable-static
make -j4
make install
```

---

# A larger project

```text
project/
├── CMakeLists.txt
├── apps/       # Applications and their CMakeLists.txt.
├── src/        # Library sources and their CMakeLists.txt.
├── tests/      # Test sources and their CMakeLists.txt.
├── cmake/      # Optional custom find modules.
├── doc/        # Documentation and templates.
└── scripts/    # Helper scripts.
```

Use `add_subdirectory` for directories that define targets. A custom find-module directory can be added with `list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")`.

Sources: [CMake documentation](https://cmake.org/cmake/help/latest/), [Modern CMake](https://cliutils.gitlab.io/modern-cmake/).
