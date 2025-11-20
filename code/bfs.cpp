#include <cstdint>
#include <print>
#include <queue>
#include <string>
#include <vector>

#include "benchmark.hpp"
#include "parallel_bfs.hpp"
#include "utils.hpp"

using std::queue;
using std::vector;

/*graph_t graph = {{1, 2}, {0, 2}, {0, 1, 3, 4}, {2}, {2}};*/
/*graph_t graph = {{2, 3}, {2}, {0, 1}, {0}, {5}, {4}};*/

vector<int64_t> bfs(graph_t& graph) {
    auto used = vector<bool>(graph.size(), false);
    auto distances = vector<int64_t>(graph.size(), -1);
    queue<int64_t> q;
    q.push(0);
    used[0] = true;
    distances[0] = 0;

    while (!q.empty()) {
        int64_t cur = q.front();
        q.pop();

        for (auto neighbor : graph[cur]) {
            if (!used[neighbor]) {
                q.push(neighbor);
                used[neighbor] = true;
                distances[neighbor] = distances[cur] + 1;
            }
        }
    }

    return distances;
}

void test() {
    graph_t graph = read_graph("graph_test.txt");
    auto dst = bfs(graph);

    // for (uint64_t i = 1; i < dst.size(); i++) {
    //     if (dst[i] != -1) {
    //         std::println("Distance between vertices 0 and {} is {}", i,
    //         dst[i]);
    //     } else {
    //         std::println("Vertex {} cannot be reached from vertex 0", i);
    //     }
    // }
}

void test2() {
    graph_t graph = read_graph("graph_test.txt");
    auto dst = parallel_bfs(graph);

    // for (uint64_t i = 1; i < dst.size(); i++) {
    //     if (dst[i] != -1) {
    //         std::println("Distance between vertices 0 and {} is {}", i,
    //         dst[i]);
    //     } else {
    //         std::println("Vertex {} cannot be reached from vertex 0", i);
    //     }
    // }
}

int main(int argc, char** argv) {
    bool parallel = false;
    if (argc > 1) {
        if (std::string(argv[1]) == "-p") {
            parallel = true;
        }
    }
    if (parallel) {
        auto [cycles, ns] = benchmark(test2);
    } else {
        auto [cycles, ns] = benchmark(test);
    }
    return 0;
}
