#ifndef BENCHMARK_H
#define BENCHMARK_H
#include <cstdint>
#include <tuple>
#include <vector>

std::tuple<std::vector<uint64_t>, std::vector<uint64_t>> benchmark(void (*)(),
                                                                   int repeats);
#endif
