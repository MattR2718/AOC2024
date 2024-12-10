#include "../utils/utils.h"
#include <set>

inline int get_index(int x, int y, int width) {
    return y * width + x;
}

int solve(const std::string_view& s, int width, const std::pair<int, int>& p, int n, uint8_t *ends, bool p2 = false) {
    int height = s.length() / width;

    if (p.first < 0 || p.first >= width || p.second < 0 || p.second >= height) {
        return 0;
    }

    int current_index = get_index(p.first, p.second, width);
    if (s[current_index] == '9' && (p2 || !(*(ends+get_index(p.first, p.second, width))))) {
        if (!p2) *(ends + get_index(p.first, p.second, width)) = 1;
        return 1;
    }

    int r = 0;
    for (const auto& [dx, dy] : aoc_utils::cardinal_directions) {
        int nx = p.first + dx;
        int ny = p.second + dy;
        if (nx >= 0 && nx < width && ny >= 0 && ny < height && s[get_index(nx, ny, width)] == '0' + n) {
            r += solve(s, width, { nx, ny }, n + 1, ends, p2);
        }
    }
    return r;
}

struct State {
    int x, y;      // Current position
    int n;         // Current number we're looking for
};


int solve_it(const std::string_view& s, int width, const std::pair<int, int>& p, int n, uint8_t* ends, bool p2 = false) {
    int height = s.length() / width;

    int sp = 0;
    std::vector<State> sta(10);
	sta[sp++] = { p.first, p.second, n };

    int ret = 0;

    while (sp) {

		State st = sta[--sp];

        if (st.x < 0 || st.x >= width || st.y < 0 || st.y >= height) {
            continue;
        }

        int current_index = get_index(st.x, st.y, width);

        if (s[current_index] == '9') {
            if (p2) {
                ret += 1;
            }
            else if (!ends[current_index]) {
                ends[current_index] = 1;
                ret += 1;
            }
            continue;
        }

        for (const auto& [dx, dy] : aoc_utils::cardinal_directions) {
            int nx = st.x + dx;
            int ny = st.y + dy;

            if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                s[get_index(nx, ny, width)] == '0' + st.n) {
                //stack.push({ nx, ny, st.n + 1});
				sta[sp++] = { nx, ny, st.n + 1 };
            }
        }
    }

    return ret;
}


int main() {
    INITIALIZE_AOC_TIMERS();
    default_timer.begin(0);

	std::string input = aoc_utils::memory_map_file_boost("input.txt").value();

	int width = input.find('\n');
	input.erase(std::remove(input.begin(), input.end(), '\n'));
	int height = input.size() / width;

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
			std::vector<uint8_t> ends(width * height, 0);
            return solve_it(input, width, s, 1, ends.data());
        });
    default_timer.end(1);

    default_timer.begin(2);
    p2 = std::transform_reduce(std::execution::unseq, starts.begin(), starts.end(), 0, std::plus<>(),
        [&](const auto& s) {
            return solve_it(input, width, s, 1, nullptr, true);
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
//Elapsed Time : 113.6 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 88.3 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 62.7 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 18
//Ticks : 184788
//TotalDays : 2.13875E-07
//TotalHours : 5.133E-06
//TotalMinutes : 0.00030798
//TotalSeconds : 0.0184788
//TotalMilliseconds : 18.4788