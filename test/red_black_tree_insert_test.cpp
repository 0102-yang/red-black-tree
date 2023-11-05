#include <gtest/gtest.h>

#include "red_black_tree.h"

TEST(InsertTests, MultipleElementsInsertTest)
{
    rbt::RedBlackTree<int, int> tree;
    std::array arr{10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55};

    ASSERT_EQ(tree.IsEmpty(), true);

    // Insertion.
    for (const int& e : arr) {
        tree.Insert(e, e + 1);
        tree.PrintTree();
    }
    ASSERT_EQ(tree.IsEmpty(), false);
    ASSERT_EQ(tree.Size(), arr.size());

    for (const int& e : arr) {
        tree.Insert(e, e + 2);
    }
    ASSERT_EQ(tree.Size(), arr.size());

    // Get value.
    {
        for (const int& e : arr) {
            auto [value, flag] = tree.GetValue(e);
            ASSERT_EQ(flag, true);
            ASSERT_EQ(value, e + 1);
        }
    }
    {
        auto [value, flag] = tree.GetValue(88);
        ASSERT_EQ(flag, false);
    }
}