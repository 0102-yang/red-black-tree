#include <gtest/gtest.h>

#include "red_black_tree.h"
#include "test_constant.h"

TEST(DeleteTests, SimpleDeleteTest)
{
    rbt::RedBlackTree<int, int> tree;
    tree.Insert(42, 42);
    tree.Insert(100, 100);

    // Deletion.
    ASSERT_FALSE(tree.Erase(65));
    ASSERT_TRUE(tree.Erase(42));
    ASSERT_TRUE(tree.RedBlackTreeRulesCheck());
    {
        const auto value = tree.GetValue(42);
        ASSERT_FALSE(value.has_value());
    }
    {
        const auto value = tree.GetValue(100);
        ASSERT_TRUE(value.has_value());
    }
    ASSERT_EQ(tree.Size(), 1);

    ASSERT_TRUE(tree.Erase(100));
    ASSERT_TRUE(tree.RedBlackTreeRulesCheck());
    ASSERT_TRUE(tree.IsEmpty());
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
        ASSERT_TRUE(tree.Erase(e));
        ASSERT_TRUE(tree.RedBlackTreeRulesCheck());
    }
}

TEST(DeleteTests, OrderedElementsDeleteTests)
{
    rbt::RedBlackTree<int, int> tree;
    for (int i = 0; i < test_size; i++) {
        tree.Insert(i, i * i);
    }

    for (int i = 0; i < test_size; i++) {
        ASSERT_TRUE(tree.Erase(i));
        const auto value = tree.GetValue(i);
        ASSERT_FALSE(value.has_value());
        ASSERT_EQ(tree.Size(), test_size - i - 1);
        ASSERT_TRUE(tree.RedBlackTreeRulesCheck());
    }
    ASSERT_TRUE(tree.IsEmpty());
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
            ASSERT_FALSE(value.has_value());
            ASSERT_TRUE(tree.RedBlackTreeRulesCheck());
        }
    }
}