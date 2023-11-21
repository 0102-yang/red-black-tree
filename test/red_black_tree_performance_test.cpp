#include <gtest/gtest.h>

#include <chrono>
#include <random>

#include "red_black_tree.h"

constexpr int iterate_time = 10000000;

TEST(PerformanceTest, OrderedElementsInsertAndGetTest)
{
    // std::map.
    auto begin = std::chrono::steady_clock().now();
    {
        std::map<int, int> m;
        for (int i = 0; i < iterate_time; ++i) {
            m.insert({i, i});
        }
        for (int i = 0; i < iterate_time; ++i) {
            auto it = m.find(i);
        }
    }
    auto end = std::chrono::steady_clock().now();
    const auto map_time = std::chrono::duration<double>(end - begin).count();

    // red-black-tree.
    begin = std::chrono::steady_clock().now();
    {
        rbt::RedBlackTree<int, int> t;
        for (int i = 0; i < iterate_time; ++i) {
            t.Insert(i, i);
        }
        for (int i = 0; i < iterate_time; ++i) {
            auto [it, flag] = t.GetValue(i);
        }
    }
    end = std::chrono::steady_clock().now();
    const auto tree_time = std::chrono::duration<double>(end - begin).count();

    std::cout << "Ordered elements: Map time is " << map_time << " second(s).\n";
    std::cout << "Ordered elements: Tree time is " << tree_time << " second(s).\n";
}

TEST(PerformanceTest, RandomElementsInsertAndGetTest)
{
    std::mt19937 mt;
    std::uniform_int_distribution dist(10000, 99999);

    // std::map.
    auto begin = std::chrono::steady_clock().now();
    {
        std::map<int, int> m;
        for (int i = 0; i < iterate_time; ++i) {
            int random_number = dist(mt);
            m.insert({random_number, random_number});
        }
        for (int i = 0; i < iterate_time; ++i) {
            int random_number = dist(mt);
            auto it = m.find(random_number);
        }
    }
    auto end = std::chrono::steady_clock().now();
    const auto map_time = std::chrono::duration<double>(end - begin).count();

    // red-black-tree.
    begin = std::chrono::steady_clock().now();
    {
        rbt::RedBlackTree<int, int> t;
        for (int i = 0; i < iterate_time; ++i) {
            int random_number = dist(mt);
            t.Insert(random_number, random_number);
        }
        for (int i = 0; i < iterate_time; ++i) {
            int random_number = dist(mt);
            auto [it, flag] = t.GetValue(random_number);
        }
    }
    end = std::chrono::steady_clock().now();
    const auto tree_time = std::chrono::duration<double>(end - begin).count();

    std::cout << "Random elements: Map time is " << map_time << " second(s).\n";
    std::cout << "Random elements: Tree time is " << tree_time << " second(s).\n";
}