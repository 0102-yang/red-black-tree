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
    add_subdirectory(third_party/googletest)
    add_subdirectory(test)
endif()

# Enable benchmark.
if(BUILD_BENCHMARK)
    add_subdirectory(bench)
endif()