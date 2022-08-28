cmake_minimum_required(VERSION 3.23)
project(ray_tracing_one_week)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
include_directories(utility)

add_executable(
        program
        program.cpp
)
