cmake_minimum_required(VERSION 3.9)
project(ray_tracing_one_week)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

include_directories(third-party)

FILE(COPY resources/earth-map.jpg DESTINATION "${CMAKE_BINARY_DIR}/resources")

add_executable(
        ray_tracing
        src/main.cpp
)