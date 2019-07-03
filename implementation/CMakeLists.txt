cmake_minimum_required(VERSION 3.5)

################################################## 
# C++ standard version selection
################################################## 
set(CXX_STD 17 CACHE STRING "Set to 17, 2a, or 20.")
message("-- Using -std=c++${CXX_STD}")


###############################################################################
# GoogleTest
###############################################################################
add_subdirectory(${CMAKE_SOURCE_DIR}/googletest-release-1.8.0)
target_include_directories(gtest      INTERFACE ${CMAKE_HOME_DIRECTORY}/googletest-release-1.8.0/googletest/include)
target_include_directories(gtest_main INTERFACE ${CMAKE_HOME_DIRECTORY}/googletest-release-1.8.0/googletest/include)


include(CTest)

enable_testing()

add_custom_target(
    check
    COMMAND ${CMAKE_CTEST_COMMAND} -j4 -C ${CMAKE_CFG_INTDIR}
)

add_executable(flat_map_test test.cpp)
target_compile_options(flat_map_test PRIVATE -Wall)
set_property(TARGET flat_map_test PROPERTY CXX_STANDARD ${CXX_STD})
target_link_libraries(flat_map_test gtest gtest_main)
add_test(flat_map_test ${CMAKE_BINARY_DIR}/flat_map_test --gtest_catch_exceptions=1)
