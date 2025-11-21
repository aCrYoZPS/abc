#include "csv_writer.hpp"

#include <format>
#include <fstream>

CsvWriter::CsvWriter() { this->buffer = "Iters;Times\n"; }
void CsvWriter::WriteValues(int iters, std::vector<uint64_t>& values) {
    for (auto value : values) {
        this->buffer.append(std::format("{};{}\n", iters, value));
    }
}

void CsvWriter::WriteToFile(std::string file_name) {
    std::ofstream fout(file_name);
    fout << buffer;
}
