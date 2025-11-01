#include <cstdint>
#include <fstream>
#include <print>
#include <queue>
#include <string>
#include <vector>

#include "utils.h"

typedef std::vector<std::vector<int64_t>> graph_t;

using std::queue;
using std::vector;

/*graph_t graph = {{1, 2}, {0, 2}, {0, 1, 3, 4}, {2}, {2}};*/
/*graph_t graph = {{2, 3}, {2}, {0, 1}, {0}, {5}, {4}};*/
vector<bool> used;
vector<int64_t> dst;

graph_t read_graph(std::string path) {
    std::ifstream ifstream(path);
    auto lines = vector<std::string>();
    graph_t graph = graph_t();
    if (ifstream.is_open()) {
        std::string line;
        int64_t i = 0;
        while (std::getline(ifstream, line)) {
            auto nums = split_string(std::move(line), ",");
            graph.push_back(vector<int64_t>());
            for (auto& num_str : nums) {
                graph[i].push_back(std::stoll(num_str));
            }
            ++i;
        }
        ifstream.close();
    }

    return graph;
}

int main() {
    graph_t graph = read_graph("graph_test.txt");

    used = vector<bool>(graph.size(), false);
    dst = vector<int64_t>(graph.size(), -1);

    queue<int64_t> q;
    q.push(0);
    used[0] = true;
    dst[0] = 0;

    while (!q.empty()) {
        int64_t cur = q.front();
        q.pop();

        for (auto neighbor : graph[cur]) {
            if (!used[neighbor]) {
                q.push(neighbor);
                used[neighbor] = true;
                dst[neighbor] = dst[cur] + 1;
            }
        }
    }

    for (uint64_t i = 1; i < dst.size(); i++) {
        if (dst[i] != -1) {
            std::println("Distance between vertices 0 and {} is {}", i, dst[i]);
        } else {
            std::println("Vertex {} cannot be reached from vertex 0", i);
        }
    }

    return 0;
}
