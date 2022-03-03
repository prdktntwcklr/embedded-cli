cmake_minimum_required(VERSION 3.10)

# Set the output directories, this... mostly works
set(cmake_build "cmake_build/")
set(EXECUTABLE_OUTPUT_PATH ${cmake_build})
set(CMAKE_BUILD_FILES_DIRECTORY ${cmake_build})
set(CMAKE_BUILD_DIRECTORY ${cmake_build})

# Declare the project
project(CLI)

# Get the source files from the src/ dir
file(GLOB_RECURSE SRC_FILES src/*.c)

# Get the main file from the example/ dir and create executable
add_executable(CLI_Example examples/main.c ${SRC_FILES})