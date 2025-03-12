#include <spdlog/spdlog.h>

#include <chrono>
#include <iostream>
#include <map>

#include "red_black_tree.h"

int main()
{
#ifndef NDEBUG
    spdlog::warn("Running benchmark in debug mode is not recommended.");
#endif

    constexpr int iterate_time = 10000000;
    rbt::IntRandomNumberGenerator gen(0, 999999);
    // *********************************************
    // Ordered elements read store test.
    // *********************************************
    // std::map.
    auto start_point = std::chrono::steady_clock::now();
    {
        std::map<int, int> m;
        for (int i = 0; i < iterate_time; ++i) {
            m.emplace(i, i);
        }
        for (int i = 0; i < iterate_time; ++i) {
            const auto it = m.find(i);
        }
        for (int i = 0; i < iterate_time; ++i) {
            const auto flag = m.erase(i);
        }
    }
    auto end_point = std::chrono::steady_clock::now();
    auto map_time = std::chrono::duration<double>(end_point - start_point).count();

    // red-black-tree.
    start_point = std::chrono::steady_clock::now();
    {
        rbt::RedBlackTree<int, int> t;
        for (int i = 0; i < iterate_time; ++i) {
            t.Insert(i, i);
        }
        for (int i = 0; i < iterate_time; ++i) {
            const auto value = t.GetValue(i);
        }
        for (int i = 0; i < iterate_time; ++i) {
            const auto flag = t.Erase(i);
        }
    }
    end_point = std::chrono::steady_clock::now();
    auto tree_time = std::chrono::duration<double>(end_point - start_point).count();

    std::cout << std::format("Ordered insert-delete {} elements: Map time is {} second(s).\n", iterate_time, map_time);
    std::cout << std::format("Ordered insert-delete {} elements: Tree time is {} second(s).\n", iterate_time, tree_time);

    // *********************************************
    // Random elements read store test.
    // *********************************************
    // std::map.
    start_point = std::chrono::steady_clock::now();
    {
        std::map<int, int> m;
        for (int i = 0; i < iterate_time; ++i) {
            const int random_number = gen();
            m.emplace(random_number, random_number);
        }
        for (int i = 0; i < iterate_time; ++i) {
            const int random_number = gen();
            const auto it = m.find(random_number);
        }
        for (int i = 0; i < iterate_time; ++i) {
            const int random_number = gen();
            const auto flag = m.erase(random_number);
        }
    }
    end_point = std::chrono::steady_clock::now();
    map_time = std::chrono::duration<double>(end_point - start_point).count();

    // red-black-tree.
    start_point = std::chrono::steady_clock::now();
    {
        rbt::RedBlackTree<int, int> t;
        for (int i = 0; i < iterate_time; ++i) {
            const int random_number = gen();
            t.Insert(random_number, random_number);
        }
        for (int i = 0; i < iterate_time; ++i) {
            const int random_number = gen();
            const auto value = t.GetValue(random_number);
        }
        for (int i = 0; i < iterate_time; ++i) {
            const int random_number = gen();
            const auto flag = t.Erase(random_number);
        }
    }
    end_point = std::chrono::steady_clock::now();
    tree_time = std::chrono::duration<double>(end_point - start_point).count();

    std::cout << std::format("Random insert-delete {} elements: Map time is {} second(s).\n", iterate_time, map_time);
    std::cout << std::format("Random insert-delete {} elements: Tree time is {} second(s).\n", iterate_time, tree_time);
}
