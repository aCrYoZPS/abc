#ifndef PARALLEL_BFS_H
#define PARALLEL_BFS_H
#include <cstdint>
#include <vector>

#include "utils.hpp"

std::vector<int64_t> parallel_bfs(graph_t& graph);
#endif  // !PARALLEL_BFS_H
