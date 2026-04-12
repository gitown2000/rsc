#!/bin/sh
set -e
# Compile
mkdir -p bin
cc -O2 -std=c11 -o bin/bubble_sort ../bubble_sort.c 2>/dev/null || cc -O2 -std=c11 -o bin/bubble_sort bubble_sort.c
# Test input and expected output
input="5 3 4 1 2"
expected="1 2 3 4 5"
out=$(printf "%s" "$input" | ./bin/bubble_sort)
if [ "$out" != "$expected" ]; then
  echo "Test failed: expected '$expected' got '$out'"
  exit 1
fi
printf "OK\n"
