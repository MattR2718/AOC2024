#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <tuple>

using namespace std;

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ (hash2 << 1); // Combine hashes
    }
};


using Grid = vector<vector<char>>;
using Mapping = unordered_map<pair<int, int>, char, pair_hash>;
using Region = unordered_set<pair<int, int>, pair_hash>;
using SymbolRegion = pair<char, Region>;

const string FILE_STR = "input.txt";

Grid read_lines_to_list() {
    Grid lines;
    ifstream file(FILE_STR);

    if (!file.is_open()) {
        cerr << "Failed to open file: " << FILE_STR << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        vector<char> row(line.begin(), line.end());
        lines.push_back(row);
    }

    file.close();
    return lines;
}

tuple<int, int, Mapping> grid_to_map(const Grid& grid) {
    int height = grid.size();
    int width = grid[0].size();
    Mapping mapping;

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            mapping[{row, col}] = grid[row][col];
        }
    }

    return { height, width, mapping };
}

void part_one() {
    Grid lines = read_lines_to_list();
    auto [height, width, mapping] = grid_to_map(lines);

    unordered_set<pair<int, int>, hash<pair<int, int>>> visited;
    set<SymbolRegion> regions;
    int answer = 0;

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            pair<int, int> start = { row, col };

            if (visited.count(start)) {
                continue;
            }

            char symbol = mapping[start];
            queue<pair<int, int>> to_visit;
            to_visit.push(start);

            Region curr_region;
            unordered_set<pair<int, int>, hash<pair<int, int>>> visited_for_symbol;

            while (!to_visit.empty()) {
                auto curr = to_visit.front();
                to_visit.pop();

                if (visited_for_symbol.count(curr)) {
                    continue;
                }

                visited_for_symbol.insert(curr);
                if (mapping[curr] != symbol) {
                    continue;
                }

                curr_region.insert(curr);
                visited.insert(curr);

                for (int i : {-1, 1}) {
                    pair<int, int> new_coords = { curr.first + i, curr.second };
                    if (mapping.count(new_coords) && mapping[new_coords] == symbol &&
                        !visited_for_symbol.count(new_coords) && !visited.count(new_coords)) {
                        to_visit.push(new_coords);
                    }

                    new_coords = { curr.first, curr.second + i };
                    if (mapping.count(new_coords) && mapping[new_coords] == symbol &&
                        !visited_for_symbol.count(new_coords) && !visited.count(new_coords)) {
                        to_visit.push(new_coords);
                    }
                }
            }

            regions.insert({ symbol, curr_region });
        }
    }

    for (const auto& [symbol, region] : regions) {
        int area = region.size();
        int perimeter = 0;

        for (const auto& [row, col] : region) {
            for (int i : {-1, 1}) {
                pair<int, int> to_check = { row + i, col };
                if (!mapping.count(to_check) || mapping[to_check] != symbol) {
                    ++perimeter;
                }

                to_check = { row, col + i };
                if (!mapping.count(to_check) || mapping[to_check] != symbol) {
                    ++perimeter;
                }
            }
        }

        int cost = area * perimeter;
        answer += cost;
    }

    cout << "Part 1: " << answer << endl;
}

void part_two() {
    Grid lines = read_lines_to_list();
    auto [height, width, mapping] = grid_to_map(lines);

    unordered_set<pair<int, int>, hash<pair<int, int>>> visited;
    set<SymbolRegion> regions;
    int answer = 0;

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            pair<int, int> start = { row, col };

            if (visited.count(start)) {
                continue;
            }

            char symbol = mapping[start];
            queue<pair<int, int>> to_visit;
            to_visit.push(start);

            Region curr_region;
            unordered_set<pair<int, int>, hash<pair<int, int>>> visited_for_symbol;

            while (!to_visit.empty()) {
                auto curr = to_visit.front();
                to_visit.pop();

                if (visited_for_symbol.count(curr)) {
                    continue;
                }

                visited_for_symbol.insert(curr);
                if (mapping[curr] != symbol) {
                    continue;
                }

                curr_region.insert(curr);
                visited.insert(curr);

                for (int i : {-1, 1}) {
                    pair<int, int> new_coords = { curr.first + i, curr.second };
                    if (mapping.count(new_coords) && mapping[new_coords] == symbol &&
                        !visited_for_symbol.count(new_coords) && !visited.count(new_coords)) {
                        to_visit.push(new_coords);
                    }

                    new_coords = { curr.first, curr.second + i };
                    if (mapping.count(new_coords) && mapping[new_coords] == symbol &&
                        !visited_for_symbol.count(new_coords) && !visited.count(new_coords)) {
                        to_visit.push(new_coords);
                    }
                }
            }

            regions.insert({ symbol, curr_region });
        }
    }

    for (const auto& [symbol, region] : regions) {
        int area = region.size();
        int sides = 0;

        for (const auto& [row, col] : region) {
            for (const auto& [i, j, adj1, adj2] :
                vector<tuple<int, int, pair<int, int>, pair<int, int>>>({
                    {1, 1, {0, 1}, {1, 0}},
                    {1, -1, {0, -1}, {1, 0}},
                    {-1, 1, {0, 1}, {-1, 0}},
                    {-1, -1, {0, -1}, {-1, 0}},
                    })) {
                pair<int, int> diag = { row + i, col + j };
                pair<int, int> adj_1 = { row + adj1.first, col + adj1.second };
                pair<int, int> adj_2 = { row + adj2.first, col + adj2.second };

                bool is_diag_out = !mapping.count(diag) || !region.count(diag);
                bool is_adj_1_out = !mapping.count(adj_1) || !region.count(adj_1);
                bool is_adj_2_out = !mapping.count(adj_2) || !region.count(adj_2);

                sides += (is_diag_out && is_adj_1_out && is_adj_2_out) ||
                    (is_diag_out && !is_adj_1_out && !is_adj_2_out) ||
                    (!is_diag_out && is_adj_1_out && is_adj_2_out);
            }
        }

        int cost = area * sides;
        answer += cost;
    }

    cout << "Part 2: " << answer << endl;
}

int main() {
    part_one();
    part_two();
    return 0;
}
