#include "benchmark.hpp"

#include <chrono>
#include <cstdint>
#include <ratio>
#include <vector>

static inline uint64_t rdtsc() {
    uint32_t lo, hi;
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

std::tuple<std::vector<uint64_t>, std::vector<uint64_t>> benchmark(
    void (*func)(), int repeats) {
    std::vector<uint64_t> cycles{};
    for (int i = 0; i < repeats; ++i) {
        uint64_t start_cycles = rdtsc();
        func();
        uint64_t end_cycles = rdtsc();

        cycles.push_back(end_cycles - start_cycles);
    }

    std::vector<uint64_t> times{};
    for (int i = 0; i < repeats; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<uint64_t, std::nano> duration = end - start;
        times.push_back(duration.count());
    }

    return std::tuple(cycles, times);
}
