#include <iostream>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <numeric>

long long measure_deletion_time(long long size) {
    std::unordered_map<int, int> map;
    map.reserve(size);
    for (int i = 0; i < size; ++i) {
        map[i] = i;
    }

    const int iterations = 10000;
    const int key_to_delete = size / 2;

    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        map.erase(key_to_delete);
        map.insert({ key_to_delete, key_to_delete });
    }
    auto end_time = std::chrono::high_resolution_clock::now();

    auto total_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    return total_duration.count() / iterations;
}

int main() {
    std::vector<long long> sizes = {
        100, 500, 1000, 5000, 10000,
        50000, 100000, 500000, 1000000, 5000000, 10000000
    };

    std::cout << "Size, Time(ns)" << std::endl;

    for (long long size : sizes) {
        long long avg_time = measure_deletion_time(size);
        std::cout << size << "," << avg_time << std::endl;
    }

    return 0;
}



