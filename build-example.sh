#!/usr/bin/env bash

set -eou pipefail

build_dir=build

rm -rf ${build_dir}
cmake --version
cmake -B ${build_dir}
cmake --build ${build_dir}
