#include "parallel_bfs.hpp"

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

using std::vector;

vector<int64_t> parallel_bfs(graph_t &graph) {
  int n = graph.size();
  vector<std::atomic<bool>> used(n);
  vector<int64_t> distances(n, -1);

  for (auto &u : used) {
    u.store(false);
  }

  vector<int64_t> frontier = {0};
  used[0].store(true);
  distances[0] = 0;

  std::mutex mtx;

  while (!frontier.empty()) {
    vector<int64_t> next_frontier;

    auto worker = [&](int start, int end) {
      vector<int64_t> local_next;
      for (int i = start; i < end; ++i) {
        int64_t v = frontier[i];
        for (int64_t u : graph[v]) {
          bool expected = false;
          if (used[u].compare_exchange_strong(expected, true)) {
            distances[u] = distances[v] + 1;
            local_next.push_back(u);
          }
        }
      }

      if (!local_next.empty()) {
        std::lock_guard<std::mutex> lock(mtx);
        next_frontier.insert(next_frontier.end(), local_next.begin(),
                             local_next.end());
      }
    };

    int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) {
      num_threads = 4;
    }

    vector<std::thread> threads;
    int block_size = (int)frontier.size() / num_threads + 1;
    for (int t = 0; t < num_threads; ++t) {
      int start = t * block_size;
      int end = std::min((t + 1) * block_size, (int)frontier.size());
      if (start >= end)
        break;
      threads.emplace_back(worker, start, end);
    }

    for (auto &th : threads) {
      th.join();
    }

    frontier = std::move(next_frontier);
  }

  return distances;
}
