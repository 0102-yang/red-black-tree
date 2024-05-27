#include <gtest/gtest.h>

#include <array>
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
        const auto value = tree.GetValue(42);
        ASSERT_EQ(value.has_value(), false);
    }
    {
        const auto value = tree.GetValue(100);
        ASSERT_EQ(value.has_value(), true);
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
    constexpr std::array arr{10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55, 45};

    // Insertion.
    for (const int& e : arr) {
        tree.Insert(e, e + 1);
    }

    // Deletion.
    for (const int& e : arr) {
        const auto is_deleted = tree.Erase(e);
        ASSERT_EQ(is_deleted, true);
#ifdef DEBUG
        ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
#endif
    }
}

TEST(DeleteTests, OrderedElementsDeleteTests)
{
    constexpr int size = 1000;
    rbt::RedBlackTree<int, int> tree;
    for (int i = 0; i < size; i++) {
        tree.Insert(i, i * i);
    }

    for (int i = 0; i < size; i++) {
        tree.Erase(i);
        const auto value = tree.GetValue(i);
        ASSERT_EQ(value.has_value(), false);
        ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
    }
    ASSERT_EQ(tree.IsEmpty(), true);
}

TEST(PerformanceTests, MultipleRandomElementsDeleteTest)
{
    constexpr int size = 1000;
    rbt::RedBlackTree<int, int> tree;

    // Insertion.
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution dist(10000, 99999);

    for (int i = 0; i < size; ++i) {
        const int random_number = dist(mt);
        tree.Insert(random_number, random_number * 2);
    }

    // Deletion.
    for (int i = 0; i < size; ++i) {
        const int random_number = dist(mt);
        if (const bool is_deleted = tree.Erase(random_number)) {
            const auto value = tree.GetValue(random_number);
            ASSERT_EQ(value.has_value(), false);
        }
        ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
    }
}