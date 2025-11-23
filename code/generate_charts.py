import sys

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import pandas as pd


def main():
    file_name = sys.argv[1]
    cpu1_data: pd.DataFrame = pd.read_csv(sys.argv[2], sep=";")
    cpu2_data: pd.DataFrame = pd.read_csv(sys.argv[3], sep=";")
    data_type = f"{sys.argv[2].split("_")[0]} {sys.argv[2].split(".csv")[0].split("_")[-1]}"
    num_iters = cpu1_data["Iters"].unique()

    cpu1_data["Times"] = cpu1_data["Times"] / 1000
    cpu2_data["Times"] = cpu2_data["Times"] / 1000

    average_times_cpu1 = cpu1_data.groupby("Iters")["Times"].mean()
    average_times_cpu2 = cpu2_data.groupby("Iters")["Times"].mean()

    fig, ax = plt.subplots(figsize=(8, 10))
    ax.plot(num_iters, average_times_cpu1, marker='o', linestyle='-', label='Ryzen 5 3500U')
    ax.plot(num_iters, average_times_cpu2, marker='s', linestyle='--', label='Ryzen 7 7700')

    ax.set_xlabel('Number of Nodes')
    ax.set_ylabel('Time (microseconds)')
    ax.set_title(f'Benchmarking: {data_type}')
    ax.set_xscale('log')
    ax.set_yscale('log')

    ax.set_xticks(num_iters)
    ax.set_yticks(list(average_times_cpu1) + list(average_times_cpu2))
    ax.tick_params(axis='y', labelrotation=45)

    ax.xaxis.set_major_formatter(ticker.ScalarFormatter())
    ax.yaxis.set_major_formatter(ticker.ScalarFormatter())

    ax.legend()
    plt.savefig(f"../media/{file_name}")
    plt.show()


if __name__ == "__main__":
    main()
