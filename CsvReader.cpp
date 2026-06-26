#include "CsvReader.h"
#include "CampusException.h"
#include <fstream>
#include <sstream>

std::string CsvReader::trim(const std::string& s) {
    std::size_t first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    std::size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

std::vector<std::string> CsvReader::split(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, ',')) {
        fields.push_back(trim(item));
    }
    return fields;
}

std::vector<std::vector<std::string>> CsvReader::read(const std::string& path) {
    std::ifstream fin(path);
    if (!fin) {
        throw CampusException("FILE_NOT_FOUND", "cannot open file: " + path);
    }

    std::vector<std::vector<std::string>> rows;
    std::string line;
    while (std::getline(fin, line)) {
        line = trim(line);
        if (line.empty()) continue;

        // TODO: 支持以 # 开头的注释行。
        // 当前实现会把注释行当作坏格式数据的一部分处理。
        rows.push_back(split(line));
    }
    return rows;
}
