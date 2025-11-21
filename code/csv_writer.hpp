#ifndef CSV_WRITER
#define CSV_WRITER

#include <cstdint>
#include <string>
#include <vector>
class CsvWriter {
   private:
    std::string buffer;

   public:
    CsvWriter();
    void WriteValues(int iters, std::vector<uint64_t>& values);
    void WriteToFile(std::string file_name);
};

#endif  // !CSV_WRITER
