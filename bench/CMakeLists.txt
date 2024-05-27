message(STATUS "Build benchmark.")
add_executable(
    performance
    ${PROJECT_SOURCE_DIR}/bench/performance.cpp
)
target_link_libraries(performance red_black_tree)