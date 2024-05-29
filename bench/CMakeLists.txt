message(STATUS "Build benchmark.")
add_executable(
    performance
    ${CMAKE_CURRENT_SOURCE_DIR}/performance.cpp
)
target_link_libraries(performance PRIVATE red_black_tree)