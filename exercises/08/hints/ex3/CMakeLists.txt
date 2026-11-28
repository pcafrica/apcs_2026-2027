cmake_minimum_required(VERSION 3.16)

project(matrix_benchmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(MATRIX_SIZE 200 CACHE STRING "Matrix dimension for the benchmark")
option(ENABLE_COVERAGE "Build with coverage instrumentation" OFF)

add_executable(ex3 ex3.cpp matrix.cpp matrix.hpp)
target_compile_definitions(ex3 PRIVATE MATRIX_SIZE=${MATRIX_SIZE})
target_compile_options(ex3 PRIVATE -Wall -Wextra -Wpedantic)

if(ENABLE_COVERAGE)
  target_compile_options(ex3 PRIVATE -O0 -g --coverage)
  target_link_options(ex3 PRIVATE --coverage)

  add_custom_target(coverage
    COMMAND lcov --directory "${CMAKE_CURRENT_BINARY_DIR}" --zerocounters
    COMMAND $<TARGET_FILE:ex3>
    COMMAND lcov --directory "${CMAKE_CURRENT_BINARY_DIR}" --capture
                 --exclude "*/include/c++/*"
                 --output-file test_coverage.info
    COMMAND genhtml test_coverage.info --output-directory test_coverage
    DEPENDS ex3
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    VERBATIM
  )
else()
  target_compile_options(ex3 PRIVATE -O2 -g)
endif()
