#include <gtest/gtest.h>

#include <chrono>
#include <random>

#include "red_black_tree.h"

TEST(PerformanceTest, GetValueTest)
{
    std::map<int, int> m;
    rbt::RedBlackTree<int, int> t;

    // std::map.
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < 1000000; ++i) {
        m[i] = i * 2;
    }
    for (int i = 0; i < 1000000; ++i) {
        auto it = m.find(i);
        std::cout << it->second << '\n';
    }
    auto end = std::chrono::steady_clock::now();
    const std::chrono::duration<double> map_duration = end - start;

    // red-black-tree.
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < 1000000; ++i) {
        t.Insert(i, i * 2);
    }
    for (int i = 0; i < 1000000; ++i) {
        auto [it, flag] = t.GetValue(i);
        std::cout << it << '\n';
    }
    end = std::chrono::steady_clock::now();
    const std::chrono::duration<double> tree_duration = end - start;
    std::cout << "Find 1000000 elements in std::map using " << map_duration.count() << " second(s)." << std::endl;
    std::cout << "Find 1000000 elements in rbt::tree using " << tree_duration.count() << " second(s)." << std::endl;
}