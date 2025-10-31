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
        while (std::getline(ifstream, line)) {
            lines.push_back(std::move(line));
        }
        ifstream.close();
    }

    return graph;
}

int main(int argc, char** argv) {
    auto res = split_string("1:1,2,3,4", const std::string& delimiter) return 0;
    graph_t graph = read_graph("path");

    used = vector<bool>(graph.size(), false);
    dst = vector<int64_t>(graph.size(), -1);

    queue<int> q;
    q.push(0);
    used[0] = true;
    dst[0] = 0;

    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        for (int neighbor : graph[cur]) {
            if (!used[neighbor]) {
                q.push(neighbor);
                used[neighbor] = true;
                dst[neighbor] = dst[cur] + 1;
            }
        }
    }

    for (int i = 1; i < dst.size(); i++) {
        if (dst[i] != -1) {
            std::println("Distance between vertices 0 and {} is {}", i, dst[i]);
        } else {
            std::println("Vertex {} cannot be reached from vertex 0", i);
        }
    }

    return 0;
}
