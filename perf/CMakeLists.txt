cmake_minimum_required(VERSION 3.0)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions(--stdlib=libc++ -std=c++1z -O3)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_definitions(-std=c++1y -O3)
endif ()

add_executable(perf_test ${CMAKE_SOURCE_DIR}/map_insertion_perf_tests.cpp)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_link_libraries(perf_test c++)
endif ()

find_package(PythonInterp)

set(perf_test_output
    boost_flat_map.py
    std_map.py
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
        ${CMAKE_SOURCE_DIR}/../paper/motivation_and_scope.in.tex
)
