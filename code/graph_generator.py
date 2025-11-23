import random
from pathlib import Path


def generate_spanning_tree(n):
    adjacency = [set() for _ in range(n)]
    nodes = list(range(n))
    random.shuffle(nodes)
    for i in range(1, n):
        j = random.randint(0, i - 1)
        adjacency[nodes[i]].add(nodes[j])
        adjacency[nodes[j]].add(nodes[i])
    return adjacency


def add_random_edges(adjacency, edge_probability):
    n = len(adjacency)
    for i in range(n):
        for j in range(i + 1, n):
            if j not in adjacency[i] and random.random() < edge_probability:
                adjacency[i].add(j)
                adjacency[j].add(i)


def generate_graph(n, edge_probability=0.00035):
    adjacency = generate_spanning_tree(n)
    add_random_edges(adjacency, edge_probability)
    return adjacency


def main():
    node_counts = [100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000]
    for node_count in node_counts:
        adjacency = generate_graph(node_count)
        file_path = Path(f"graphs/graph_{node_count}.txt")
        file_path.parent.mkdir(parents=True, exist_ok=True)

        with open(file_path, "w") as f:
            for neighbors in adjacency:
                line = ",".join(map(str, sorted(neighbors)))
                f.write(line + "\n")


if __name__ == "__main__":
    main()
