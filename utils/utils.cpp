#include "utils.h"
#include <boost/iostreams/device/mapped_file.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>

namespace aoc_utils {

    // Cardinal and diagonal direction vectors
    const std::vector<std::pair<int, int>> cardinal_directions = {
        {0, -1}, {-1, 0}, {0, 1}, {1, 0}
    };

    const std::vector<std::pair<int, int>> diagonal_directions = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    const std::vector<std::pair<int, int>> all_directions = {
        {0, -1}, {-1, 0}, {0, 1}, {1, 0},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    std::optional<std::string> memory_map_file(const std::string& file_path) {
        std::ifstream file(file_path, std::ios::in | std::ios::binary);
        if (!file.is_open()) return std::nullopt;

        std::ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::vector<std::string> tokenize(const std::string& input, const std::string& delimiters) {
        std::vector<std::string> tokens;
        size_t start = 0, end = 0;
        while ((end = input.find_first_of(delimiters, start)) != std::string::npos) {
            if (end != start) {
                tokens.push_back(input.substr(start, end - start));
            }
            start = end + 1;
        }
        if (start < input.size()) {
            tokens.push_back(input.substr(start));
        }
        return tokens;
    }

    std::vector<std::string> read_lines(const std::string& file_path) {
        std::vector<std::string> lines;
        std::ifstream file(file_path);
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        return lines;
    }

    std::vector<std::string> read_lines_mmap(const std::string filename) {
        boost::iostreams::mapped_file_source file;
        file.open(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        const char* data = file.data();
        size_t size = file.size();

        std::vector<std::string> results;
        const char* line_start = data;

        for (size_t i = 0; i < size; ++i) {
            if (data[i] == '\n' || i == size - 1) {
                size_t line_length = (data[i] == '\n') ? (data + i - line_start) : (data + i - line_start + 1);
                std::string line(line_start, line_length);

                // Remove carriage return if it exists at the end of the line
                if (!line.empty() && line.back() == '\r') {
                    line.pop_back();
                }

                results.push_back(line);
                line_start = data + i + 1;
            }
        }

        file.close();
        return results;
    }

    void read_lines_mmap_apply(const std::string filename, std::function<void(const std::string&)> apply) {
        boost::iostreams::mapped_file_source file;
        file.open(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        const char* data = file.data();
        size_t size = file.size();

        const char* line_start = data;

        for (size_t i = 0; i < size; ++i) {
            if (data[i] == '\n' || i == size - 1) {
                size_t line_length = (data[i] == '\n') ? (data + i - line_start) : (data + i - line_start + 1);
                std::string line(line_start, line_length);

                // Remove carriage return if it exists at the end of the line
                if (!line.empty() && line.back() == '\r') {
                    line.pop_back();
                }

                apply(line);
                line_start = data + i + 1;
            }
        }

        file.close();
    }
}
