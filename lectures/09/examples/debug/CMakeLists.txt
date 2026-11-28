cmake_minimum_required(VERSION 3.16)

project(debug_examples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

foreach(example IN ITEMS assert1 assert2 debug_macro defensive pause print
                         student1 student2 undefined_behaviour)
  add_executable(${example} ${example}.cpp)
  target_compile_options(${example} PRIVATE -O0 -g -Wall -Wextra -Wpedantic)
endforeach()
