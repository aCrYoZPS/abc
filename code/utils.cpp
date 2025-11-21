#include "utils.hpp"

#include <cstdint>
#include <fstream>

std::vector<std::string> split_string(const std::string& s,
                                      const std::string& delimiter) {
    if (s.length() == 0) {
        return {};
    }
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start));

    return tokens;
}

graph_t read_graph(std::string path) {
    std::ifstream ifstream(path);
    graph_t graph = graph_t();
    if (ifstream.is_open()) {
        std::string line;
        int64_t i = 0;
        while (std::getline(ifstream, line)) {
            auto nums = split_string(std::move(line), ",");
            graph.push_back(std::vector<int64_t>());
            for (auto& num_str : nums) {
                graph[i].push_back(std::stoll(num_str));
            }
            ++i;
        }
        ifstream.close();
    }

    return graph;
}
