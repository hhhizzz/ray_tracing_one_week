cmake_minimum_required(VERSION 3.23)
project(ray_tracing_one_week)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
include_directories(utility)

add_executable(
        main
        program.cpp
        utility/camera.h
        utility/color.h
        utility/hittable.h
        utility/hittable_list.h
        utility/material.h
        utility/ray.h
        utility/rtweekend.h
        utility/sphere.h
        utility/vec3.h
)
