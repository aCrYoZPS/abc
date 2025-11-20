import sys

import matplotlib.pyplot as plt
import pandas as pd


def main():
    file_name = sys.argv[1]
    cpu1_data = pd.read_csv(sys.argv[2], sep=";")
    cpu2_data = pd.read_csv(sys.argv[3], sep=";")
    data_type = sys.argv[2].split(".csv")[0].split("_")[-1]
    num_iters = cpu1_data["Iters"]

    times_cpu1 = cpu1_data["Times"]
    times_cpu2 = cpu2_data["Times"]

    plt.plot(num_iters, times_cpu1, marker='o', linestyle='-', label='CPU 1')
    plt.plot(num_iters, times_cpu2, marker='s', linestyle='--', label='CPU 2')

    plt.xlabel('Number of Nodes')
    plt.ylabel('Time (nanoseconds)')
    plt.title(f'Benchmarking: {data_type}')
    plt.xscale('log')
    plt.yscale('log')
    plt.legend()
    plt.savefig(f"../media/{file_name}")
    plt.show()


if __name__ == "__main__":
    main()
