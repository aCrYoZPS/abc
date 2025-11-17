#ifndef PARALLEL_BFS_H
#define PARALLEL_BFS_H
#include <vector>

typedef std::vector<std::vector<int64_t>> graph_t;

std::vector<int64_t> parallel_bfs(graph_t& graph);
#endif  // !PARALLEL_BFS_H
