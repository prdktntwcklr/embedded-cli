#!/usr/bin/env bash

# Script to run a pristine build from inside the Docker container

rm -rf build
cmake --version
cmake -B build
cmake --build build
