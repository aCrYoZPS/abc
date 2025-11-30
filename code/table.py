import pandas as pd


def main():
    cpu1_data: pd.DataFrame = pd.read_csv("dfs_laptop_smt.csv", sep=";")
    cpu2_data: pd.DataFrame = pd.read_csv("dfs_pc_smt.csv", sep=";")
    cpu1_data = cpu1_data[cpu1_data["Iters"] >= 1000]
    cpu2_data = cpu2_data[cpu2_data["Iters"] >= 1000]
    num_iters = cpu1_data["Iters"].unique()

    cpu1_data["Times"] = cpu1_data["Times"] / 1000
    cpu2_data["Times"] = cpu2_data["Times"] / 1000

    average_times_cpu1 = cpu1_data.groupby("Iters")["Times"].mean()
    average_times_cpu2 = cpu2_data.groupby("Iters")["Times"].mean()

    print(num_iters)
    print(average_times_cpu1)
    print(average_times_cpu2)
    print((average_times_cpu1 / average_times_cpu2).mean())


if __name__ == "__main__":
    main()


383.6374
407.3502
336.1356
428.9151
1055.7595
4658.8191
19427.4481
