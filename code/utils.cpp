#include "utils.hpp"

#include <chrono>
#include <cstdint>
#include <fstream>
#include <ratio>

std::vector<std::string> split_string(const std::string& s,
                                      const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start));

    return tokens;
}
static inline uint64_t rdtsc() {
    uint32_t lo, hi;
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

std::tuple<uint64_t, uint64_t> benchmark(void (*func)()) {
    uint64_t start_cycles = rdtsc();
    func();
    uint64_t end_cycles = rdtsc();

    auto total_cycles = end_cycles - start_cycles;

    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<uint64_t, std::nano> duration = end - start;
    auto total_nanoseconds = duration.count();

    return std::tuple(total_cycles, total_nanoseconds);
}

graph_t read_graph(std::string path) {
    std::ifstream ifstream(path);
    graph_t graph = graph_t();
    if (ifstream.is_open()) {
        std::string line;
        int64_t i = 0;
        while (std::getline(ifstream, line)) {
            auto nums = split_string(std::move(line), ",");
            graph.push_back(std::vector<int64_t>());
            for (auto& num_str : nums) {
                graph[i].push_back(std::stoll(num_str));
            }
            ++i;
        }
        ifstream.close();
    }

    return graph;
}
