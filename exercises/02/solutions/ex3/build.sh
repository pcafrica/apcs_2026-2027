#!/bin/bash

set -x

if g++ -std=c++20 -Wall -Wextra -Wpedantic statistics.cpp main.cpp -o main; then
    set +x
    echo "Build successful! You can run the program using ./main"
else
    status=$?
    set +x
    echo "Build failed." >&2
    exit "$status"
fi
