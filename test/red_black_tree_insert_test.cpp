import <array>;
import <random>;

#include <gtest/gtest.h>

#include "test_constant.h"

import red_black_tree;

TEST(InsertTests, EmptyTest)
{
    rbt::RedBlackTree<int, int> tree;
    ASSERT_EQ(tree.IsEmpty(), true);
    ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
}

TEST(InsertTests, SimpleInsertTest)
{
    rbt::RedBlackTree<int, int> tree;
    ASSERT_EQ(tree.IsEmpty(), true);

    tree.Insert(1, 1);
    ASSERT_EQ(tree.IsEmpty(), false);
    ASSERT_EQ(tree.Size(), 1);
    ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);

    tree.Insert(2, 2);
    ASSERT_EQ(tree.IsEmpty(), false);
    ASSERT_EQ(tree.Size(), 2);
    ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);

    tree.Insert(1, 1);
    ASSERT_EQ(tree.IsEmpty(), false);
    ASSERT_EQ(tree.Size(), 2);
    ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
}

TEST(InsertTests, MultipleElementsInsertTest)
{
    rbt::RedBlackTree<int, int> tree;

    // Insertion.
    for (const int& e : classic_array) {
        tree.Insert(e, e + 1);
        ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
    }
    ASSERT_EQ(tree.Size(), classic_array.size());

    // Duplicate key insertion.
    for (const int& e : classic_array) {
        tree.Insert(e, e + 2);
    }
    ASSERT_EQ(tree.Size(), classic_array.size());

    // Get value.
    {
        for (const int& e : classic_array) {
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
    rbt::RedBlackTree<int, int> tree;
    for (int i = 0; i < test_size; i++) {
        tree.Insert(i, i * i);
        ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
    }

    for (int i = 0; i < test_size; i++) {
        auto value = tree.GetValue(i);
        ASSERT_EQ(value.has_value(), true);
        ASSERT_EQ(*value, i * i);
    }

    for (int i = test_size; i < test_size * 2; i++) {
        auto value = tree.GetValue(i);
        ASSERT_EQ(value.has_value(), false);
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
        ASSERT_EQ(tree.RedBlackTreeRulesCheck(), true);
    }

    for (const auto [k, v] : num_table) {
        auto value = tree.GetValue(k);
        ASSERT_EQ(value.has_value(), true);
        ASSERT_EQ(*value, v);
    }
}