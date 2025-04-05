cmake_minimum_required(VERSION 3.20)

# export compile commands for clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(cli_example)

set(CMAKE_C_STANDARD 99 CACHE STRING "The C standard to use")

set(SRC_FILES src/cli.c)
add_executable(cli_example examples/main.c ${SRC_FILES})
target_include_directories(cli_example PRIVATE src)

if(TARGET_GROUP STREQUAL test)
  enable_testing()
  add_subdirectory(tests)
endif()
