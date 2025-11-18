#include <hip/hip_runtime.h>

#include <iostream>
#include <limits>
#include <vector>

#include "utils.hpp"

#define INF 1000000000

__global__ void bfs_kernel(int n, const int* row_offsets,
                           const int* col_indices, int* distances,
                           int* frontier, int frontier_size, int* next_frontier,
                           int* next_frontier_size) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < frontier_size) {
        int u = frontier[tid];
        int dist_u = distances[u];
        int start = row_offsets[u];
        int end = row_offsets[u + 1];
        for (int i = start; i < end; i++) {
            int v = col_indices[i];
            if (atomicCAS(&distances[v], INF, dist_u + 1) == INF) {
                int pos = atomicAdd(next_frontier_size, 1);
                next_frontier[pos] = v;
            }
        }
    }
}

std::vector<int> bfs_hip(int n, const graph_t& adj_list, int root) {
    std::vector<int> row_offsets(n + 1, 0);
    int edge_count = 0;
    for (int i = 0; i < n; i++) {
        row_offsets[i + 1] = row_offsets[i] + adj_list[i].size();
        edge_count += adj_list[i].size();
    }
    std::vector<int> col_indices(edge_count);
    int idx = 0;
    for (int i = 0; i < n; i++) {
        for (int v : adj_list[i]) {
            col_indices[idx++] = v;
        }
    }

    int *d_row_offsets, *d_col_indices, *d_distances;
    hipMalloc(&d_row_offsets, (n + 1) * sizeof(int));
    hipMalloc(&d_col_indices, edge_count * sizeof(int));
    hipMalloc(&d_distances, n * sizeof(int));

    hipMemcpy(d_row_offsets, row_offsets.data(), (n + 1) * sizeof(int),
              hipMemcpyHostToDevice);
    hipMemcpy(d_col_indices, col_indices.data(), edge_count * sizeof(int),
              hipMemcpyHostToDevice);

    std::vector<int> distances(n, INF);
    distances[root] = 0;

    int *d_frontier, *d_next_frontier, *d_next_frontier_size;
    hipMalloc(&d_frontier, n * sizeof(int));
    hipMalloc(&d_next_frontier, n * sizeof(int));
    hipMalloc(&d_next_frontier_size, sizeof(int));

    hipMemcpy(d_frontier, &root, sizeof(int), hipMemcpyHostToDevice);
    int frontier_size = 1;
    hipMemcpy(d_distances, distances.data(), n * sizeof(int),
              hipMemcpyHostToDevice);

    while (frontier_size > 0) {
        hipMemset(d_next_frontier_size, 0, sizeof(int));

        int block_size = 256;
        int grid_size = (frontier_size + block_size - 1) / block_size;

        hipLaunchKernelGGL(bfs_kernel, dim3(grid_size), dim3(block_size), 0, 0,
                           n, d_row_offsets, d_col_indices, d_distances,
                           d_frontier, frontier_size, d_next_frontier,
                           d_next_frontier_size);

        hipMemcpy(&frontier_size, d_next_frontier_size, sizeof(int),
                  hipMemcpyDeviceToHost);

        std::swap(d_frontier, d_next_frontier);
    }

    hipMemcpy(distances.data(), d_distances, n * sizeof(int),
              hipMemcpyDeviceToHost);

    hipFree(d_row_offsets);
    hipFree(d_col_indices);
    hipFree(d_distances);
    hipFree(d_frontier);
    hipFree(d_next_frontier);
    hipFree(d_next_frontier_size);

    return distances;
}

int main() {
    graph_t graph = read_graph("graph_test.txt");
    int n = graph.size();
    int root = 0;
    auto dst = bfs_hip(n, graph, root);
    std::cout << "Distances from root " << root << ":\n";
    for (int i = 0; i < n; i++) {
        std::cout << "Node " << i << ": "
                  << (dst[i] == INF ? "INF" : std::to_string(dst[i])) << "\n";
    }
    return 0;
}
