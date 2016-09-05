cmake_minimum_required(VERSION 3.0)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

if (NOT MSVC)
    add_compile_options(-std=c++1y)
endif ()

add_executable(perf_test ${CMAKE_SOURCE_DIR}/map_insertion_perf_tests.cpp)

find_package(PythonInterp)

#message("PYTHONINTERP_FOUND=${PYTHONINTERP_FOUND}")
#message("PYTHON_EXECUTABLE=${PYTHON_EXECUTABLE}")

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
        "Running perf tests..."
)
#    ${PYTHON_EXECUTABLE}

add_custom_target(
    make_tex_file_from_data
    ALL
    DEPENDS
        ${perf_test_output}
)
