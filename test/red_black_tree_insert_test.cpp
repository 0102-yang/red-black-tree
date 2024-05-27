#include <gtest/gtest.h>

#include <array>
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
    // Array from Data Structures and Algorithm Analysis in C++ (Fourth Edition) by Mark Allen Weiss.
    constexpr std::array arr{10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55, 45};

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
            auto value = tree.GetValue(e);
            ASSERT_EQ(value.has_value(), true);
            ASSERT_EQ(*value, e + 1);
        }
    }
    {
        const auto value = tree.GetValue(88);
        ASSERT_EQ(value.has_value(), false);
    }
}

TEST(InsertTests, OrderedElementsInsertTests)
{
    constexpr int size = 1000;
    rbt::RedBlackTree<int, int> tree;
    for (int i = 0; i < size; i++) {
        tree.Insert(i, i * i);
        ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
    }

    for (int i = 0; i < size; i++) {
        auto value = tree.GetValue(i);
        ASSERT_EQ(value.has_value(), true);
        ASSERT_EQ(*value, i * i);
    }
}

TEST(InsertTests, MultipleRandomElementsInsertTest)
{
    constexpr int size = 10000;
    rbt::RedBlackTree<int, int> tree;

    // Insertion.
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution dist(10000, 99999);

    std::unordered_map<int, int> num_table;
    for (int i = 0; i < size; ++i) {
        const int random_number = dist(mt);
        const int value = random_number * 2;
        tree.Insert(random_number, value);
        num_table.emplace(random_number, value);
    }

    ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);

    for (const auto [k, v] : num_table) {
        auto value = tree.GetValue(k);
        ASSERT_EQ(value.has_value(), true);
        ASSERT_EQ(*value, v);
    }
}