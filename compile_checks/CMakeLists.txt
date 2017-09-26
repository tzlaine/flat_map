cmake_minimum_required(VERSION 3.0)

add_compile_options(-std=c++17 -stdlib=libc++)

include_directories(${CMAKE_SOURCE_DIR}/../paper)

add_executable(motivation ${CMAKE_SOURCE_DIR}/test.cpp)
