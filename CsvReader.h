#ifndef CSV_READER_H
#define CSV_READER_H

#include <string>
#include <vector>

class CsvReader {
public:
    static std::vector<std::vector<std::string>> read(const std::string& path);
    static std::vector<std::string> split(const std::string& line);
    static std::string trim(const std::string& s);
};

#endif
