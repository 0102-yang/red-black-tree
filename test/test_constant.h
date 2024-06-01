#pragma once

import <array>;
import <random>;

constexpr int test_size = 1000;
// Array from Data Structures and Algorithm Analysis in C++ (Fourth Edition) by Mark Allen Weiss.
constexpr std::array classic_array{10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55, 45};

class IntRandomNumberGenerator
{
public:
    IntRandomNumberGenerator(const int min, const int max) : dist_(min, max), gen_(std::random_device()()) {}

    int operator()() { return dist_(gen_); }

private:
    std::uniform_int_distribution<> dist_;
    std::mt19937 gen_;
};