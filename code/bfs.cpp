#include <cstddef>
#include <cstdint>
#include <print>
#include <queue>
#include <string>
#include <vector>

#include "benchmark.hpp"
#include "csv_writer.hpp"
#include "parallel_bfs.hpp"
#include "utils.hpp"

using std::queue;
using std::vector;

int repeats = 10;
std::vector<int> node_counts = {10000, 20000, 50000, 100000};
int current_node_count = 10000;

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
    std::string file_name =
        std::format("graphs/graph_{}.txt", current_node_count);
    graph_t graph = read_graph(file_name);
    volatile auto dst = bfs(graph);
}

void test2() {
    std::string file_name =
        std::format("graphs/graph_{}.txt", current_node_count);
    graph_t graph = read_graph(file_name);
    volatile auto dst = parallel_bfs(graph);
}

int main(int argc, char** argv) {
    bool parallel = false;
    CsvWriter csv_writer{};
    if (argc > 2) {
        auto argv2 = std::string(argv[2]);
        if (argv2.starts_with("-p")) {
            parallel = true;
        }
    }

    std::string result_file_name = std::format("bfs_{}", argv[1]);

    void (*test_func)();
    if (parallel) {
        test_func = test2;
        result_file_name += "-multithreaded.csv";
    } else {
        test_func = test;
        result_file_name += "-singlethreaded.csv";
    }

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
