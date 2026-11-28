cmake_minimum_required(VERSION 3.16)

project(optimization_examples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

foreach(example IN ITEMS static loop_unrolling data_alignment)
  add_executable(${example} ${example}.cpp)
  target_compile_options(${example} PRIVATE -O3 -Wall -Wextra -Wpedantic)
endforeach()
