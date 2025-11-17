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

bool dfs(graph_t& graph, int64_t v, int64_t p = -1) {
    used[v] = true;

    for (int64_t u : graph[v]) {
        if (!used[u]) {
            if (!dfs(graph, u, v)) {
                return false;
            }
        } else if (u != p) {
            return false;
        }
    }

    return true;
}

bool has_cycles(graph_t& graph) {
    for (auto i = 0; i < graph.size(); i++) {
        if (!used[i]) {
            if (!dfs(graph, i)) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    int64_t n = 0;
    graph_t graph;
    if (has_cycles(graph)) {
        std::println("Graph has a cycle");
    } else {
        std::println("Graph has no cycles");
    }
}
