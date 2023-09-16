add_executable(
  red_black_tree_insert_test
  red_black_tree_insert_test.cpp
)
add_executable(
  red_black_tree_delete_test
  red_black_tree_delete_test.cpp
)

target_link_libraries(
  red_black_tree_insert_test
  GTest::gtest_main
  red_black_tree
)
target_link_libraries(
  red_black_tree_delete_test
  GTest::gtest_main
  red_black_tree
)

include(GoogleTest)
gtest_discover_tests(red_black_tree_insert_test)
gtest_discover_tests(red_black_tree_delete_test)