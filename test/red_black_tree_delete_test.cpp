#include <gtest/gtest.h>

#include <random>

#include "red_black_tree.h"

TEST(DeleteTests, SimpleDeleteTest)
{
    rbt::RedBlackTree<int, int> tree;
    tree.Insert(42, 42);
    tree.Insert(100, 100);

    // Deletion.
    ASSERT_EQ(tree.Erase(65), false);
    ASSERT_EQ(tree.Erase(42), true);
    {
        auto [_, flag] = tree.GetValue(42);
        ASSERT_EQ(flag, false);
    }
    {
        auto [_, flag] = tree.GetValue(100);
        ASSERT_EQ(flag, true);
    }
    ASSERT_EQ(tree.Size(), 1);
#ifdef DEBUG
    ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
#endif

    tree.Erase(100);
#ifdef DEBUG
    ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
#endif
    ASSERT_EQ(tree.IsEmpty(), true);
}

TEST(DeleteTests, MultipleElementsDeleteTest)
{
    rbt::RedBlackTree<int, int> tree;
    std::array arr{10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55, 45};

    // Insertion.
    for (const int& e : arr) {
        tree.Insert(e, e + 1);
    }

    // Deletion.
    for (const int& e : arr) {
        auto flag = tree.Erase(e);
        ASSERT_EQ(flag, true);
#ifdef DEBUG
        ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
#endif
    }
}

TEST(DeleteTests, OrderedElementsDeleteTests)
{
    rbt::RedBlackTree<int, int> tree;
    for (int i = 0; i < 20; i++) {
        tree.Insert(i, i * i);
    }

    for (int i = 0; i < 20; i++) {
        tree.Erase(i);
        ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
    }
    ASSERT_EQ(tree.IsEmpty(), true);
}