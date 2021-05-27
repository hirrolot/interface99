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
fi
