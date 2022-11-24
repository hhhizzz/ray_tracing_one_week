cmake_minimum_required(VERSION 3.9)

if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 75)
endif()

project(ray_tracing_one_week LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)
include_directories(third-party)

FILE(COPY resources/earth-map.jpg DESTINATION "${CMAKE_BINARY_DIR}/resources")

add_executable(
        ray_tracing
        src/main.cpp
)

target_compile_features(ray_tracing PUBLIC cxx_std_17)

set_target_properties(ray_tracing PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)
