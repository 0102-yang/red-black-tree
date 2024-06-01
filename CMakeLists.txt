cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(RedBlackTree LANGUAGES CXX VERSION 1.0)

message(STATUS "Build operating System: ${CMAKE_SYSTEM}")
message(STATUS "Build mode: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compile flags (Debug): ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "C++ compile flags (Release): ${CMAKE_CXX_FLAGS_RELEASE}")

option(BUILD_TESTS "Build red-black-tree essential tests." $<CONFIG:Debug>)
option(BUILD_BENCHMARK "Build red-black-tree performance benchmark." OFF)

# Build spdlog.
add_subdirectory(third_party/spdlog)

# Build red black tree.
add_subdirectory(src)

# Enable test.
if(BUILD_TESTS)
    add_subdirectory(third_party/googletest)
    add_subdirectory(test)
endif()

# Enable benchmark.
if(BUILD_BENCHMARK)
    add_subdirectory(bench)
endif()