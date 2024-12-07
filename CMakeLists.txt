cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Aktina VERSION 1.0)

add_executable(aktina src/main.cpp)
target_include_directories(aktina PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
