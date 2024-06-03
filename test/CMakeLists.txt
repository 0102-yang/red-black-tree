message(STATUS "Building red black tree tests.")
enable_testing()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(install_gtest OFF)
set(install_gmock OFF)
set(build_gmock ON)

include(GoogleTest)

add_executable(
    red_black_tree_insert_test
    red_black_tree_insert_test.cpp
    test_main.cpp)
add_executable(
    red_black_tree_delete_test
    red_black_tree_delete_test.cpp
    test_main.cpp)

target_compile_definitions(red_black_tree_insert_test PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_definitions(red_black_tree_delete_test PRIVATE $<$<CONFIG:Debug>:DEBUG>)

target_link_libraries(red_black_tree_insert_test PRIVATE red_black_tree PRIVATE gtest PRIVATE gtest_main)
target_link_libraries(red_black_tree_delete_test PRIVATE red_black_tree PRIVATE gtest PRIVATE gtest_main)

gtest_discover_tests(red_black_tree_insert_test)
gtest_discover_tests(red_black_tree_delete_test)