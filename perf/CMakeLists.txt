cmake_minimum_required(VERSION 3.0)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(perf_test ${CMAKE_SOURCE_DIR}/map_insertion_perf_tests.cpp)
set_property(TARGET perf_test PROPERTY CXX_STANDARD 14)

find_package(PythonInterp)

set(perf_test_output
    boost_flat_map.py
    std_map.py
    vector.py
    vector_custom_pair.py
)

add_custom_command(
    OUTPUT
        ${perf_test_output}
    COMMAND
        perf_test
    DEPENDS
        perf_test
    COMMENT
        "-- Running perf tests..."
)

add_custom_target(
    make_data
    DEPENDS
        ${perf_test_output}
    COMMENT
        "-- Filling in LaTex graphs..."
)

add_custom_target(
    make_tex_file_from_data
    ALL
    COMMAND
        ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/make_tex.py
    DEPENDS
        make_data
        ${CMAKE_CURRENT_SOURCE_DIR}/make_tex.py
        ${CMAKE_SOURCE_DIR}/paper.in.tex
)
