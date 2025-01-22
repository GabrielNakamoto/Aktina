cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(aktina VERSION 1.0)

add_executable(aktina src/main.cpp src/thread_worker.cpp)

target_compile_options(aktina PRIVATE -O2 -Wall)

target_include_directories(aktina PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
