cmake_minimum_required(VERSION 3.20)

# export compile commands for clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# declare the project
project(cli_example)

# set C standard
set(CMAKE_C_STANDARD 99 CACHE STRING "The C standard to use")

# create a SRC_FILES variable with a link to all c files to compile
set(SRC_FILES src/cli.c)

# get the main file from the example/ dir and create executable
add_executable(cli_example examples/main.c ${SRC_FILES})

# set the directories that should be included in the build command for this target
target_include_directories(cli_example PRIVATE src)
