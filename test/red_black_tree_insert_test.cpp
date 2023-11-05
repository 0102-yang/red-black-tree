#include <gtest/gtest.h>

#include "red_black_tree.h"

TEST(InsertTests, MultipleElementsInsertTest)
{
    rbt::RedBlackTree<int, int> tree;
    std::array arr{10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55};

    // Insertion.
    for (const int& e : arr) {
        tree.Insert(e, e + 1);

#ifdef DEBUG
        tree.PrintTree();
#endif
    }

    ASSERT_EQ(tree.Size(), arr.size());
}