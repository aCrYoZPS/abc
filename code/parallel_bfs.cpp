#include "parallel_bfs.hpp"

#include <algorithm>
#include <atomic>
#include <numeric>  // for std::accumulate
#include <thread>
#include <vector>

using std::vector;

vector<int64_t> parallel_bfs(graph_t& graph) {
    int n = graph.size();
    // Use int8_t (char) instead of bool to potentially reduce false sharing
    // slightly, though padding is the only real cure. Using atomic_flag or
    // atomic<bool> is fine provided we reduce contention logic.
    vector<std::atomic<bool>> used(n);
    vector<int64_t> distances(n, -1);

    // Initialization
    for (auto& u : used) {
        u.store(false, std::memory_order_relaxed);
    }

    vector<int64_t> frontier = {0};
    used[0].store(true, std::memory_order_relaxed);
    distances[0] = 0;

    // Calculate thread count once
    int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;

    // Threshold: If frontier is smaller than this, don't parallelize.
    // Thread creation cost usually outweighs benefits for < 1000-5000 nodes.
    const size_t PARALLEL_THRESHOLD = 1024;

    while (!frontier.empty()) {
        // 1. SEQUENTIAL PATH (Optimization for small frontiers)
        if (frontier.size() < PARALLEL_THRESHOLD) {
            vector<int64_t> next_frontier;
            for (int64_t v : frontier) {
                for (int64_t u : graph[v]) {
                    // Relaxed ordering is sufficient for the load
                    if (!used[u].load(std::memory_order_relaxed)) {
                        // Strong exchange ensures only one thread (if we were
                        // parallel) enters
                        bool expected = false;
                        if (used[u].compare_exchange_strong(
                                expected, true, std::memory_order_acquire)) {
                            distances[u] = distances[v] + 1;
                            next_frontier.push_back(u);
                        }
                    }
                }
            }
            frontier = std::move(next_frontier);
            continue;
        }

        // 2. PARALLEL PATH
        // Storage for each thread's results to avoid locking
        vector<vector<int64_t>> thread_next_frontiers(num_threads);
        vector<std::thread> threads;

        int block_size = (frontier.size() + num_threads - 1) / num_threads;

        auto worker = [&](int t_id, int start, int end) {
            // Access thread-local vector directly
            vector<int64_t>& local_next = thread_next_frontiers[t_id];

            // Reserve memory to prevent frequent reallocations during push_back
            // A heuristic guess can reduce overhead (e.g., avg degree is often
            // small)
            local_next.reserve(block_size * 2);

            for (int i = start; i < end; ++i) {
                int64_t v = frontier[i];
                for (int64_t u : graph[v]) {
                    // Double-check locking pattern optimization
                    // First check without atomic op (cheap read)
                    if (!used[u].load(std::memory_order_relaxed)) {
                        bool expected = false;
                        // Then attempt the atomic write
                        if (used[u].compare_exchange_strong(
                                expected, true, std::memory_order_acquire)) {
                            distances[u] = distances[v] + 1;
                            local_next.push_back(u);
                        }
                    }
                }
            }
        };

        for (int t = 0; t < num_threads; ++t) {
            int start = t * block_size;
            int end = std::min((int64_t)(start + block_size),
                               (int64_t)frontier.size());
            if (start >= end) break;
            threads.emplace_back(worker, t, start, end);
        }

        for (auto& th : threads) {
            th.join();
        }

        // 3. MERGE RESULTS (No Mutex Needed)
        // Calculate total size needed
        size_t total_size = 0;
        for (const auto& vec : thread_next_frontiers) total_size += vec.size();

        vector<int64_t> next_frontier;
        next_frontier.reserve(total_size);

        // Flatten vectors
        for (auto& vec : thread_next_frontiers) {
            next_frontier.insert(next_frontier.end(), vec.begin(), vec.end());
        }

        frontier = std::move(next_frontier);
    }

    return distances;
}
