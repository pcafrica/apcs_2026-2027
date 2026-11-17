#!/bin/bash

set -x

mkdir -p build

if c++ -std=c++20 -Wall -Wextra -Wpedantic -Iinclude/ \
       src/calculator.cpp -c -o build/calculator.o && \
   c++ -std=c++20 -Wall -Wextra -Wpedantic -Iinclude/ \
       src/main.cpp -c -o build/main.o && \
   c++ build/calculator.o build/main.o -o build/calculator; then
    set +x
    echo "Build successful! You can run the program using ./build/calculator"
else
    status=$?
    set +x
    echo "Build failed." >&2
    exit "$status"
fi
