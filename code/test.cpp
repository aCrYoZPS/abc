#include <cstdint>
#include <vector>

#include "csv_writer.hpp"

int main() {
    CsvWriter writer{};
    auto values1 = std::vector<uint64_t>{1, 2, 3, 5};
    writer.WriteValues(10, values1);
    auto values2 = std::vector<uint64_t>{6, 7, 9, 31};
    writer.WriteValues(20, values2);
    writer.WriteToFile("test.csv");
}
