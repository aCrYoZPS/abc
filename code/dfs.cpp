#include <cstdint>
#include <fstream>
#include <print>
#include <queue>
#include <string>
#include <vector>

#include "utils.hpp"

using std::queue;
using std::vector;

/*graph_t graph = {{1, 2}, {0, 2}, {0, 1, 3, 4}, {2}, {2}};*/
/*graph_t graph = {{2, 3}, {2}, {0, 1}, {0}, {5}, {4}};*/
vector<bool> used;
vector<int64_t> dst;

int32_t dfs(graph_t& graph, int64_t v, int64_t p = -1) {
    used[v] = true;

    for (int64_t u : graph[v]) {
        if (!used[u]) {
            if (dfs(graph, u, v) == -1) {
                return -1;
            }
        } else if (u != p) {
            return -1;
        }
    }

    return 0;
}

int main() {
    int64_t n = 0;
    graph_t graph;
    for (int i = 0; i < n; i++) {
        if (!used[i]) {
            if (dfs(graph, i) == -1) {
                std::println("Graph has a cycle");
                return 0;
            }
        }
    }

    std::println("Graph has no cycles");
}
