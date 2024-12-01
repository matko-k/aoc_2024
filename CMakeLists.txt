cmake_minimum_required(VERSION 3.30)
project(aoc_2024)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

file(GLOB_RECURSE SRC_FILES "src/**/*.cpp")
add_executable(aoc_2024 src/main.cpp ${SRC_FILES}
        src/commons.h
        src/commons.cpp)
