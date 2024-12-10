#include "../utils/utils.h"
#include <set>

inline int get_index(int x, int y, int width) {
    return y * width + x;
}

int solve(const std::string_view& s, int width, const std::pair<int, int>& p, int n, std::set<std::pair<int, int>>& ends) {
    int height = s.length() / width;

    if (p.first < 0 || p.first >= width || p.second < 0 || p.second >= height) {
        return 0;
    }

    int current_index = get_index(p.first, p.second, width);
    if (s[current_index] == '9' && !ends.contains(p)) {
        ends.insert(p);
        return 1;
    }

    int r = 0;
    for (const auto& [dx, dy] : aoc_utils::cardinal_directions) {
        int nx = p.first + dx;
        int ny = p.second + dy;
        if (nx >= 0 && nx < width && ny >= 0 && ny < height && s[get_index(nx, ny, width)] == '0' + n) {
            r += solve(s, width, { nx, ny }, n + 1, ends);
        }
    }
    return r;
}

int solve2(const std::string_view& s, int width, const std::pair<int, int>& p, int n) {
    int height = s.length() / width;

    if (p.first < 0 || p.first >= width || p.second < 0 || p.second >= height) {
        return 0;
    }

    int current_index = get_index(p.first, p.second, width);
    if (s[current_index] == '9') {
        return 1;
    }

    int r = 0;
    for (const auto& [dx, dy] : aoc_utils::cardinal_directions) {
        int nx = p.first + dx;
        int ny = p.second + dy;
        if (nx >= 0 && nx < width && ny >= 0 && ny < height && s[get_index(nx, ny, width)] == '0' + n) {
            r += solve2(s, width, { nx, ny }, n + 1);
        }
    }
    return r;
}

int main() {
    INITIALIZE_AOC_TIMERS();
    default_timer.begin(0);

	std::string input = aoc_utils::memory_map_file_boost("input.txt").value();

	int width = input.find('\n');
	input.erase(std::remove(input.begin(), input.end(), '\n'));
	int height = input.size() / width;

    // Find starting positions
    std::vector<std::pair<int, int>> starts;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            if (input[get_index(i, j, width)] == '0') {
                starts.push_back({ i, j });
            }
        }
    }
    default_timer.end(0);

    int p1 = 0, p2 = 0;
    default_timer.begin(1);
    p1 = std::transform_reduce(std::execution::unseq, starts.begin(), starts.end(), 0, std::plus<>(),
        [&](const auto& s) {
            std::set<std::pair<int, int>> ends;
            return solve(input, width, s, 1, ends);
        });
    default_timer.end(1);

    default_timer.begin(2);
    p2 = std::transform_reduce(std::execution::unseq, starts.begin(), starts.end(), 0, std::plus<>(),
        [&](const auto& s) {
            return solve2(input, width, s, 1);
        });
    default_timer.end(2);

    std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';
    default_timer.display_all();
}

//Part 1: 514
//Part 2 : 1162
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 99.6 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 112.2 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 62.1 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 26
//Ticks : 260754
//TotalDays : 3.01798611111111E-07
//TotalHours : 7.24316666666667E-06
//TotalMinutes : 0.00043459
//TotalSeconds : 0.0260754
//TotalMilliseconds : 26.0754