#!/usr/bin/env bash

set -eou pipefail

build_dir=build

rm -rf ${build_dir}
mkdir -p ${build_dir}
cmake -DTARGET_GROUP=test -S . -B ${build_dir}
cmake --build ${build_dir}
ctest --test-dir ${build_dir}/tests/ -V
