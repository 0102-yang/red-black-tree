CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
PROJECT(RedBlackTree LANGUAGES C CXX VERSION 1.0)

# Default build type is Debug.
IF (NOT CMAKE_BUILD_TYPE)
    MESSAGE(STATUS "Setting build type to `Debug` as none was specified.")
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF ()
message("Build mode: ${CMAKE_BUILD_TYPE}")

# Debug settings.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Set address sanitizer.
    set(SANITIZER address)
    message("${SANITIZER} sanitizer will be enabled in debug mode.")

    # Compiler flags.
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-parameter -Wno-attributes")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fsanitize=${SANITIZER} -fno-omit-frame-pointer -fno-optimize-sibling-calls")
    message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")

    # Add debug macros.
    add_definitions(-DDEBUG)
endif()

# Release settings.
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    # Compiler flags.
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
    message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
endif()

## Cmake modules.
add_subdirectory(src)

# Enable test.
enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_subdirectory(test)