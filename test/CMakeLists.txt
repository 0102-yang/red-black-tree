message(STATUS "Building red black tree tests.")

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(install_gtest OFF)
set(install_gmock OFF)
set(build_gmock ON)

include(GoogleTest)

function(add_and_configure_test test_name test_source)
    add_executable(
        ${test_name}
        ${test_source}
        test_main.cpp)
    target_compile_definitions(${test_name} PRIVATE $<$<CONFIG:Debug>:DEBUG>)
    target_link_libraries(${test_name} PRIVATE red_black_tree PRIVATE gtest PRIVATE gtest_main)
    gtest_discover_tests(${test_name})
endfunction()

add_and_configure_test(red_black_tree_insert_test red_black_tree_insert_test.cpp)
add_and_configure_test(red_black_tree_delete_test red_black_tree_delete_test.cpp)