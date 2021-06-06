#!/bin/bash

mkdir -p examples/build
cd examples/build
cmake ..
cmake --build .

run_example() {
    echo "executing ./$1 ..."
    ./$1
}

if [[ "$OSTYPE" == "linux-gnu" ]]; then
    run_example "state"
    run_example "trace_state"
    run_example "airplane"
    run_example "read_write"
fi
