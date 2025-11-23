#include "benchmark.hpp"

#include <chrono>
#include <cstdint>
#include <ratio>

uint64_t benchmark(void (*func)()) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<uint64_t, std::nano> duration = end - start;
    return duration.count();
}
