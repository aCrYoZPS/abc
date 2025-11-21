#ifndef UTILS_H
#define UTILS_H
#include <cstdint>
#include <string>
#include <vector>

typedef std::vector<std::vector<int64_t>> graph_t;

std::vector<std::string> split_string(const std::string& s,
                                      const std::string& delimiter);

graph_t read_graph(std::string path);
#endif  // !UTILS_H
