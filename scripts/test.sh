#!/bin/bash

mkdir -p tests/build
cd tests/build
cmake ..
cmake --build .

run_test() {
    echo "executing ./$1 ..."
    ./$1
}

if [[ "$OSTYPE" == "linux-gnu" ]]; then
    run_test "basic_tests"
    run_test "decl_impl"
    run_test "impl_primary"
    run_test "metalang99_compliant"
    run_test "static_impl"
    run_test "superinterfaces"
fi
