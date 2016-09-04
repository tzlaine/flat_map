cmake_minimum_required(VERSION 3.0)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

if (NOT MSVC)
    add_compile_options(-std=c++1y)
endif ()

add_executable(perf_test ${CMAKE_SOURCE_DIR}/map_insertion_perf_tests.cpp)
