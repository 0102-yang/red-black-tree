#include <chrono>
#include <iostream>
#include <map>
#include <random>

#include "red_black_tree.h"

int main()
{
    constexpr int iterate_time = 10000000;
    // *********************************************
    // Ordered elements read store test.
    // *********************************************
    // std::map.
    auto start_point = std::chrono::steady_clock().now();
    {
        std::map<int, int> m;
        for (int i = 0; i < iterate_time; ++i) {
            m.insert({i, i});
        }
        for (int i = 0; i < iterate_time; ++i) {
            auto it = m.find(i);
        }
    }
    auto end_point = std::chrono::steady_clock().now();
    auto map_time = std::chrono::duration<double>(end_point - start_point).count();

    // red-black-tree.
    start_point = std::chrono::steady_clock().now();
    {
        rbt::RedBlackTree<int, int> t;
        for (int i = 0; i < iterate_time; ++i) {
            t.Insert(i, i);
        }
        for (int i = 0; i < iterate_time; ++i) {
            auto [it, flag] = t.GetValue(i);
        }
    }
    end_point = std::chrono::steady_clock().now();
    auto tree_time = std::chrono::duration<double>(end_point - start_point).count();

    std::cout << "Ordered elements: Map time is " << map_time << " second(s).\n";
    std::cout << "Ordered elements: Tree time is " << tree_time << " second(s).\n";

    // *********************************************
    // Random elements read store test.
    // *********************************************
    std::mt19937 mt;
    std::uniform_int_distribution dist(10000, 99999);

    // std::map.
    start_point = std::chrono::steady_clock().now();
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
    end_point = std::chrono::steady_clock().now();
    map_time = std::chrono::duration<double>(end_point - start_point).count();

    // red-black-tree.
    start_point = std::chrono::steady_clock().now();
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
    end_point = std::chrono::steady_clock().now();
    tree_time = std::chrono::duration<double>(end_point - start_point).count();

    std::cout << "Random elements: Map time is " << map_time << " second(s).\n";
    std::cout << "Random elements: Tree time is " << tree_time << " second(s).\n";
}
