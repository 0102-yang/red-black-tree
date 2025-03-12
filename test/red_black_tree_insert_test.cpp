#include <gtest/gtest.h>

#include <array>

#include "red_black_tree.h"
#include "test_constant.h"

TEST(InsertTests, EmptyTest)
{
    rbt::RedBlackTree<int, int> tree;
    ASSERT_TRUE(tree.IsEmpty());
    ASSERT_TRUE(tree.RedBlackTreeRulesCheck());
}

TEST(InsertTests, SimpleInsertTest)
{
    rbt::RedBlackTree<int, int> tree;
    ASSERT_TRUE(tree.IsEmpty());

    ASSERT_TRUE(tree.Insert(1, 1));
    ASSERT_FALSE(tree.IsEmpty());
    ASSERT_EQ(tree.Size(), 1);
    ASSERT_TRUE(tree.RedBlackTreeRulesCheck());

    ASSERT_TRUE(tree.Insert(2, 2));
    ASSERT_FALSE(tree.IsEmpty());
    ASSERT_EQ(tree.Size(), 2);
    ASSERT_TRUE(tree.RedBlackTreeRulesCheck());

    // Duplicate key insertion.
    ASSERT_FALSE(tree.Insert(1, 1));
    ASSERT_FALSE(tree.IsEmpty());
    ASSERT_EQ(tree.Size(), 2);
    ASSERT_TRUE(tree.RedBlackTreeRulesCheck());
}

TEST(InsertTests, MultipleElementsInsertTest)
{
    rbt::RedBlackTree<int, int> tree;

    // Insertion.
    for (const int& e : classic_array) {
        ASSERT_TRUE(tree.Insert(e, e + 1));
        ASSERT_TRUE(tree.RedBlackTreeRulesCheck());
    }
    ASSERT_EQ(tree.Size(), classic_array.size());

    // Duplicate key insertion.
    for (const int& e : classic_array) {
        ASSERT_FALSE(tree.Insert(e, e + 2));
    }
    ASSERT_EQ(tree.Size(), classic_array.size());

    // Get value.
    {
        for (const int& e : classic_array) {
            auto value = tree.GetValue(e);
            ASSERT_TRUE(value.has_value());
            ASSERT_EQ(*value, e + 1);
        }
    }
    {
        const auto value = tree.GetValue(88);
        ASSERT_FALSE(value.has_value());
    }
}

TEST(InsertTests, OrderedElementsInsertTests)
{
    rbt::RedBlackTree<int, int> tree;
    for (int i = 0; i < test_size; i++) {
        ASSERT_TRUE(tree.Insert(i, i * i));
        ASSERT_TRUE(tree.RedBlackTreeRulesCheck());
    }

    for (int i = 0; i < test_size; i++) {
        auto value = tree.GetValue(i);
        ASSERT_TRUE(value.has_value());
        ASSERT_EQ(*value, i * i);
    }

    for (int i = test_size; i < test_size * 2; i++) {
        auto value = tree.GetValue(i);
        ASSERT_FALSE(value.has_value());
    }
}

TEST(InsertTests, MultipleRandomElementsInsertTest)
{
    rbt::RedBlackTree<int, int> tree;
    rbt::IntRandomNumberGenerator rng(0, 999);
    std::unordered_map<int, int> num_table;

    // Insertion.
    for (int i = 0; i < test_size; ++i) {
        const int random_number = rng();
        const int value = random_number * 2;
        tree.Insert(random_number, value);
        num_table.emplace(random_number, value);
        ASSERT_TRUE(tree.RedBlackTreeRulesCheck());
    }

    for (const auto [k, v] : num_table) {
        auto value = tree.GetValue(k);
        ASSERT_TRUE(value.has_value());
        ASSERT_EQ(*value, v);
    }
}