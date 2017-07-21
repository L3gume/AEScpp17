cmake_minimum_required (VERSION 3.0)
project(AES)

file(GLOB_RECURSE sources src/*.cpp src/*.h)

add_executable(AES ${sources})
target_include_directories(AES PUBLIC src)
