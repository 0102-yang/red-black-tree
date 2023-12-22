cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(RedBlackTree LANGUAGES CXX VERSION 1.0)

message(STATUS "Build operating System: ${CMAKE_SYSTEM}")
message(STATUS "Build mode: ${CMAKE_BUILD_TYPE}")

option(BUILD_TESTS "Build red-black-tree essential tests." $<CONFIG:Debug>)
option(BUILD_BENCHMARK "Build red-black-tree performance benchmark." OFF)

## Build red-black-tree.
add_library(red_black_tree OBJECT)
target_sources(red_black_tree PRIVATE ${PROJECT_SOURCE_DIR}/src/red_black_tree.cpp)
target_compile_options(red_black_tree
    PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
            $<$<CONFIG:Debug>:-Wall -Wextra -Werror -Wno-unused-parameter -Wno-attributes -fno-omit-frame-pointer -fno-optimize-sibling-calls>
        >
)
target_compile_definitions(red_black_tree PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_include_directories(red_black_tree PUBLIC ${PROJECT_SOURCE_DIR}/src)

# Enable test.
if(BUILD_TESTS)
    message(STATUS "Enable tests.")
    enable_testing()

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(install_gtest OFF)
    set(install_gmock OFF)
    set(build_gmock ON)

    add_subdirectory(third_party/googletest)

    include(GoogleTest)
    set(TESTS_ROOT_DIR ${PROJECT_SOURCE_DIR}/test)

    add_executable(
      red_black_tree_insert_test
      ${TESTS_ROOT_DIR}/red_black_tree_insert_test.cpp
    )
    add_executable(
      red_black_tree_delete_test
      ${TESTS_ROOT_DIR}/red_black_tree_delete_test.cpp
    )

    target_compile_definitions(red_black_tree_insert_test PRIVATE $<$<CONFIG:Debug>:DEBUG>)
    target_compile_definitions(red_black_tree_delete_test PRIVATE $<$<CONFIG:Debug>:DEBUG>)

    target_link_libraries(red_black_tree_insert_test red_black_tree gtest gtest_main)
    target_link_libraries(red_black_tree_delete_test red_black_tree gtest gtest_main)

    gtest_discover_tests(red_black_tree_insert_test)
    gtest_discover_tests(red_black_tree_delete_test)
endif()

# Enable benchmark.
if(BUILD_BENCHMARK)
    message(STATUS "Build benchmark.")

    add_executable(
      performance
      ${PROJECT_SOURCE_DIR}/bench/performance.cpp
    )
    target_link_libraries(performance red_black_tree)
endif()