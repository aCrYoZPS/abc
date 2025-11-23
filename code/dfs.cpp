#include <cstdint>
#include <print>
#include <vector>

#include "benchmark.hpp"
#include "csv_writer.hpp"
#include "utils.hpp"

using std::vector;

int repeats = 10;
std::vector<int> node_counts = {100,  200,   500,   1000,  2000,
                                5000, 10000, 20000, 50000, 100000};
int current_node_count = 100;

vector<bool> used;

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
    used.assign(graph.size(), false);
    for (auto i = 0; i < graph.size(); i++) {
        if (!used[i]) {
            if (!dfs(graph, i)) {
                return true;
            }
        }
    }
    return false;
}

void test_func() {
    std::string file_name =
        std::format("graphs/graph_{}.txt", current_node_count);
    graph_t graph = read_graph(file_name);
    volatile auto cycles = has_cycles(graph);
}

int main(int argc, char** argv) {
    CsvWriter csv_writer{};
    std::string result_file_name = std::format("dfs_{}.csv", argv[1]);

    for (int i = 0; i < node_counts.size(); ++i) {
        current_node_count = node_counts[i];
        std::vector<uint64_t> times{};
        for (int i = 0; i < repeats; ++i) {
            times.push_back(benchmark(test_func));
        }
        csv_writer.WriteValues(current_node_count, times);
    }
    csv_writer.WriteToFile(result_file_name);

    return 0;
}
