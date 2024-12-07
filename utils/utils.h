#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <functional>
#include <optional>
#include <fstream>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <algorithm>
#include <regex>
#include <execution>
#include <ranges>
#include <string_view>

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include "ctre.hpp" // Assuming this is where the ctre namespace is defined
#include "fmt/core.h"

#include "timer.h"

namespace aoc_utils {

    // Cardinal and diagonal direction vectors
    static const std::vector<std::pair<int, int>> cardinal_directions = {
        {0, -1}, {-1, 0}, {0, 1}, {1, 0}
    };

    static const std::vector<std::pair<int, int>> diagonal_directions = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    static const std::vector<std::pair<int, int>> all_directions = {
        {0, -1}, {-1, 0}, {0, 1}, {1, 0},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    template <typename T, auto Pattern>
    std::vector<T> split_ctre(const std::string& input,
        std::function<T(const std::string&)> convert
    ) {
        std::vector<T> result;
        auto matches = ctre::match<Pattern>(input);
        for (const auto& match : matches) {
            // If `match` is convertible to `std::string`, use it directly
            if constexpr (std::is_convertible_v<decltype(match), std::string>) {
                std::cout << "Match type: " << typeid(match).name() << std::endl;
                std::cout << "Match value: " << std::string(match) << std::endl;
                result.push_back(convert(std::string(match)));
            }
            else {
                std::cout << "Match type: " << typeid(match).name() << std::endl;

                // If `match` is an iterable range, construct a string from the range
                if constexpr (requires { match.begin(); match.end(); }) {
                    std::string match_str(match.begin(), match.end());
                    std::cout << "Match value: " << match_str << std::endl;
                    result.push_back(convert(match_str));
                }
            }
        }
        return result;
    }


    std::optional<std::string> memory_map_file(const std::string& file_path) {
        std::ifstream file(file_path, std::ios::in | std::ios::binary);
        if (!file.is_open()) return std::nullopt;

        std::ostringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }



    std::optional<std::string> memory_map_file_boost(
        const std::string filename
    ) {
        boost::iostreams::mapped_file_source file;
        file.open(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        const char* data = file.data();
        size_t size = file.size();
		return std::string(data, size);
    }


    void memory_map_file_boost_sv(
        const std::string& filename,
        boost::iostreams::mapped_file_source& file,
        std::string_view& sv
    ) {
        file.open(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }
        sv = std::string_view(file.data(), file.size());
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

    template <typename T>
    std::vector<T> tokenize(const std::string& input, const std::string& delimiters, std::function<T(const std::string&)> convert = [](const std::string& s) { return s; }) {
        std::vector<T> tokens;
        size_t start = 0, end = 0;
        while ((end = input.find_first_of(delimiters, start)) != std::string::npos) {
            if (end != start) {
                tokens.push_back(convert(input.substr(start, end - start)));
            }
            start = end + 1;
        }
        if (start < input.size()) {
            tokens.push_back(convert(input.substr(start)));
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

    template <typename T>
    std::vector<T> read_lines_mmap(
        const std::string filename,
        std::function<T(const std::string&)> convert = [](const std::string& s) { return s; }
    ) {
        boost::iostreams::mapped_file_source file;
        file.open(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        const char* data = file.data();
        size_t size = file.size();

        std::vector<T> results;
        const char* line_start = data;

        for (size_t i = 0; i < size; ++i) {
            if (data[i] == '\n' || i == size - 1) {
                size_t line_length = (data[i] == '\n') ? (data + i - line_start) : (data + i - line_start + 1);
                std::string line(line_start, line_length);

                // Remove carriage return if it exists at the end of the line
                if (!line.empty() && line.back() == '\r') {
                    line.pop_back();
                }

                results.push_back(convert(line));
                line_start = data + i + 1;
            }
        }

        file.close();
        return results;
    }

	// Read in lines, run function on each line
    template <typename T>
    void read_lines_mmap_apply(
        const std::string filename,
        std::function<T(const std::string&)> apply
    ) {
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

    // Custom hash for pairs of integers (or any type you need)
    template<typename T>
    struct custom_hash {
        std::size_t operator()(const T& value) const {
            return std::hash<int>()(value.first) ^ (std::hash<int>()(value.second) << 1);
        }
    };

    template <typename Node, typename Cost, typename Hash = std::hash<Node>>
    std::optional<std::vector<Node>> a_star(
        Node start,
        Node goal,
        const std::function<std::vector<std::pair<Node, Cost>>(Node)>& neighbors,
        const std::function<Cost(Node, Node)>& heuristic,
        const Hash& hash_func = Hash()) {

        using PNode = std::pair<Cost, Node>;

        std::priority_queue<PNode, std::vector<PNode>, std::greater<>> open;
        std::unordered_map<Node, Cost, Hash> cost_so_far;  // Default std::equal_to for equality
        std::unordered_map<Node, Node, Hash> came_from;  // Default std::equal_to for equality

        open.emplace(0, start);
        cost_so_far[start] = 0;

        while (!open.empty()) {
            Node current = open.top().second;
            open.pop();

            if (current == goal) {
                std::vector<Node> path;
                while (current != start) {
                    path.push_back(current);
                    current = came_from[current];
                }
                path.push_back(start);
                std::reverse(path.begin(), path.end());
                return path;
            }

            for (const auto& [next, step_cost] : neighbors(current)) {
                Cost new_cost = cost_so_far[current] + step_cost;
                if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
                    cost_so_far[next] = new_cost;
                    Cost priority = new_cost + heuristic(next, goal);
                    open.emplace(priority, next);
                    came_from[next] = current;
                }
            }
        }

        return std::nullopt;
    }

    template <typename Node, typename Cost, typename Hash = std::hash<Node>>
    std::optional<std::vector<Node>> dijkstra(
        Node start,
        Node goal,
        const std::function<std::vector<std::pair<Node, Cost>>(Node)>& neighbors,
        const Hash& hash_func = Hash()) {

        using PNode = std::pair<Cost, Node>;

        std::priority_queue<PNode, std::vector<PNode>, std::greater<>> open;
        std::unordered_map<Node, Cost, Hash> cost_so_far;  // Default std::equal_to for equality
        std::unordered_map<Node, Node, Hash> came_from;  // Default std::equal_to for equality

        open.emplace(0, start);
        cost_so_far[start] = 0;

        while (!open.empty()) {
            Node current = open.top().second;
            open.pop();

            if (current == goal) {
                std::vector<Node> path;
                while (current != start) {
                    path.push_back(current);
                    current = came_from[current];
                }
                path.push_back(start);
                std::reverse(path.begin(), path.end());
                return path;
            }

            for (const auto& [next, step_cost] : neighbors(current)) {
                Cost new_cost = cost_so_far[current] + step_cost;
                if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
                    cost_so_far[next] = new_cost;
                    open.emplace(new_cost, next);
                    came_from[next] = current;
                }
            }
        }

        return std::nullopt;
    }

    template <typename Node, typename Hash = std::hash<Node>>
    std::vector<Node> depth_first_search(
        Node start,
        const std::function<std::vector<Node>(Node)>& neighbors,
        const Hash& hash_func = Hash()) {

        std::vector<Node> visited;
        std::stack<Node> stack;
        std::unordered_set<Node, Hash> seen;  // Default std::equal_to for equality

        stack.push(start);
        seen.insert(start);

        while (!stack.empty()) {
            Node current = stack.top();
            stack.pop();
            visited.push_back(current);

            for (const auto& next : neighbors(current)) {
                if (!seen.count(next)) {
                    stack.push(next);
                    seen.insert(next);
                }
            }
        }

        return visited;
    }

    template <typename Node, typename Hash = std::hash<Node>>
    std::vector<Node> breadth_first_search(
        Node start,
        const std::function<std::vector<Node>(Node)>& neighbors,
        const Hash& hash_func = Hash()) {

        std::vector<Node> visited;
        std::queue<Node> queue;
        std::unordered_set<Node, Hash> seen;  // Default std::equal_to for equality

        queue.push(start);
        seen.insert(start);

        while (!queue.empty()) {
            Node current = queue.front();
            queue.pop();
            visited.push_back(current);

            for (const auto& next : neighbors(current)) {
                if (!seen.count(next)) {
                    queue.push(next);
                    seen.insert(next);
                }
            }
        }

        return visited;
    }

}