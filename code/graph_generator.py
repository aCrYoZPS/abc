import sys
import random
from pathlib import Path


def generate_graph(n: int, edge_probability=0.3):
    adjacency = [set() for _ in range(n)]

    for i in range(n):
        for j in range(i + 1, n):
            if random.random() < edge_probability:
                adjacency[i].add(j)
                adjacency[j].add(i)

    return adjacency


def main():
    node_count = int(sys.argv[1])
    adjacency = generate_graph(node_count)
    file_path = Path(f"graphs/graph_{node_count}.txt")
    file_path.parent.mkdir(parents=True, exist_ok=True)

    with open(f"graphs/graph_{node_count}.txt", "w") as f:
        for neighbors in adjacency:
            line = ",".join(map(str, sorted(neighbors)))
            f.write(line + "\n")


if __name__ == "__main__":
    main()
