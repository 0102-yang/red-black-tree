#include <array>
#include <random>

#include <gtest/gtest.h>

#include "test_constant.h"
#include "red_black_tree.h"

TEST(DeleteTests, SimpleDeleteTest)
{
    rbt::RedBlackTree<int, int> tree;
    tree.Insert(42, 42);
    tree.Insert(100, 100);

    // Deletion.
    ASSERT_EQ(tree.Erase(65), false);
    ASSERT_EQ(tree.Erase(42), true);
    ASSERT_TRUE(tree.RedBlackTreeRulesCheck());
    {
        const auto value = tree.GetValue(42);
        ASSERT_EQ(value.has_value(), false);
    }
    {
        const auto value = tree.GetValue(100);
        ASSERT_EQ(value.has_value(), true);
    }
    ASSERT_EQ(tree.Size(), 1);

    tree.Erase(100);
    ASSERT_TRUE(tree.RedBlackTreeRulesCheck());
    ASSERT_EQ(tree.IsEmpty(), true);
}

TEST(DeleteTests, MultipleElementsDeleteTest)
{
    rbt::RedBlackTree<int, int> tree;

    // Insertion.
    for (const int& e : classic_array) {
        tree.Insert(e, e + 1);
    }

    // Deletion.
    for (const int& e : classic_array) {
        const auto is_deleted = tree.Erase(e);
        ASSERT_EQ(is_deleted, true);
        ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
    }
}

TEST(DeleteTests, OrderedElementsDeleteTests)
{
    rbt::RedBlackTree<int, int> tree;
    for (int i = 0; i < test_size; i++) {
        tree.Insert(i, i * i);
    }

    for (int i = 0; i < test_size; i++) {
        tree.Erase(i);
        const auto value = tree.GetValue(i);
        ASSERT_EQ(value.has_value(), false);
        ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
    }
    ASSERT_EQ(tree.IsEmpty(), true);
}

TEST(DeleteTests, MultipleRandomElementsDeleteTest)
{
    rbt::RedBlackTree<int, int> tree;
    rbt::IntRandomNumberGenerator rng(0, 999);

    // Insertion.
    for (int i = 0; i < test_size; ++i) {
        const int random_number = rng();
        tree.Insert(random_number, random_number * 2);
    }

    // Deletion.
    for (int i = 0; i < test_size; ++i) {
        if (const int random_number = rng(); tree.Erase(random_number)) {
            const auto value = tree.GetValue(random_number);
            ASSERT_EQ(value.has_value(), false);
            ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
        }
    }
}