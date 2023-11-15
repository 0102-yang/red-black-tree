#include <gtest/gtest.h>

#include <random>

#include "red_black_tree.h"

TEST(InsertTests, EmptyTest)
{
    rbt::RedBlackTree<int, int> tree;
    ASSERT_EQ(tree.IsEmpty(), true);
#ifdef DEBUG
    ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
#endif
}

TEST(InsertTests, SimpleInsertTest)
{
    rbt::RedBlackTree<int, int> tree;
    ASSERT_EQ(tree.IsEmpty(), true);

    tree.Insert(1, 1);
    ASSERT_EQ(tree.IsEmpty(), false);
    ASSERT_EQ(tree.Size(), 1);
#ifdef DEBUG
    ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
#endif
}

TEST(InsertTests, MultipleElementsInsertTest)
{
    rbt::RedBlackTree<int, int> tree;
    std::array arr{10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55, 45};

    // Insertion.
    for (const int& e : arr) {
        tree.Insert(e, e + 1);
#ifdef DEBUG
        ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
#endif
    }
    ASSERT_EQ(tree.Size(), arr.size());

    // Duplicate key insertion.
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

TEST(InsertTests, MultipleRandomElementsInsertTest)
{
    rbt::RedBlackTree<int, int> tree;

    // Insertion.
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution dist(10000, 99999);

    for (int i = 0; i < 1000; ++i) {
        int random_number = dist(mt);
        tree.Insert(random_number, random_number * 2);
        ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
    }
}