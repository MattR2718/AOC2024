#include "../utils/utils.h"

enum DIRECTION {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

char e_t_c(DIRECTION d) {
    switch (d) {
    case UP: return '^';
    case RIGHT: return '>';
    case DOWN: return 'v';
    case LEFT: return '<';
    }
    return ' ';
}

const std::vector<std::pair<int, int>> dirs = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1}
        
};

struct Node {
    int x, y;
    std::string path;

    bool operator<(const Node& other) const {
        return path.length() > other.path.length();
    }
};

const std::vector<std::pair<int, int>> directions = {
    {0, 1},
    {0, -1},
    {1, 0},
    {-1, 0}
};
const std::string directionChars = "><v^";

const std::vector<std::vector<char>> keypad = {
        {'7', '8', '9'},
        {'4', '5', '6'},
        {'1', '2', '3'},
        {' ', '0', 'A'}
};

const std::vector<std::vector<char>> dirpad = {
    {' ', '^', 'A'},
    {'<', 'v', '>'}
};


const std::map<char, std::pair<int, int>> num_to_pos_keypad = {
	{ '1', { 0, 2 } },
	{ '2', { 1, 2 } },
	{ '3', { 2, 2 } },
	{ '4', { 0, 1 } },
	{ '5', { 1, 1 } },
	{ '6', { 2, 1 } },
	{ '7', { 0, 0 } },
	{ '8', { 1, 0 } },
	{ '9', { 2, 0 } },
	{ '0', { 1, 3 } },
	{ 'A', { 2, 3 } }
};

const std::map<char, std::pair<int, int>> num_to_pos_dirpad = {
	{ 'A', { 2, 0 } },
	{ '>', { 2, 1 } },
	{ 'v', { 1, 1 } },
	{ '<', { 0, 1 } },
	{ '^', { 1, 0} }
};

struct State {
    int row, col;
    std::string path;
    State(int r, int c, std::string p) : row(r), col(c), path(p) {}
};


struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ (hash2 << 1);
    }
};

std::pair<int, int> find_char(const std::vector<std::vector<char>>& grid, char target) {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] == target) {
                return { i, j };
            }
        }
    }
    return { -1, -1 };
}

bool isValid(const std::vector<std::vector<char>>& grid, int row, int col) {
    return row >= 0 && row < grid.size() && col >= 0 && col < grid[0].size() && grid[row][col] != ' ';
}

std::vector<std::string> findPaths(const std::vector<std::vector<char>>& grid, char start, char end) {
    std::vector<std::string> result;

    const std::vector<std::pair<int, int>> dirs = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };
    const std::vector<char> dirChars = { '^', '>', 'v', '<' };

    auto [startRow, startCol] = find_char(grid, start);
    auto [endRow, endCol] = find_char(grid, end);

    if (startRow == -1 || endRow == -1) return result;

    std::queue<State> q;
    q.push(State(startRow, startCol, ""));

    int minLength = -1;

    while (!q.empty()) {
        State current = q.front();
        q.pop();

        if (minLength != -1 && current.path.length() > minLength) {
            break;
        }

        if (current.row == endRow && current.col == endCol) {
            if (minLength == -1) {
                minLength = current.path.length();
            }
            result.push_back(current.path);
            continue;
        }

        for (int i = 0; i < 4; i++) {
            int newRow = current.row + dirs[i].first;
            int newCol = current.col + dirs[i].second;

            if (isValid(grid, newRow, newCol)) {
                q.push(State(newRow, newCol, current.path + dirChars[i]));
            }
        }
    }

    return result;
}

std::string convert_dir_path_to_standard(const std::string& path,
    const std::vector<std::vector<char>>& dirpad) {
    std::unordered_map<char, char> dirMap;
    for (int i = 0; i < dirpad.size(); i++) {
        for (int j = 0; j < dirpad[0].size(); j++) {
            char c = dirpad[i][j];
            if (c == '^') dirMap[c] = '^';
            else if (c == 'v') dirMap[c] = 'v';
            else if (c == '<') dirMap[c] = '<';
            else if (c == '>') dirMap[c] = '>';
        }
    }
    return path;
}

std::vector<std::string> find_keypad_paths(const std::vector<std::vector<char>>& pad,
    char start, char end) {
    bool isDirPad = (pad.size() == 2 && pad[0].size() == 3 &&
        (pad[0][1] == '^' || pad[1][0] == '<'));

    auto paths = findPaths(pad, start, end);

    if (isDirPad) {
        for (auto& path : paths) {
            path = convert_dir_path_to_standard(path, pad);
        }
    }

    return paths;
}



void build_seq(std::string keys, int index, char prev_key, std::string curr_path, std::vector<std::string>& result, std::map<char, std::map<char, std::vector<std::string>>> paths) {
    if (index == keys.size()) { result.push_back(curr_path);  return; }
    for (const auto& path : paths[prev_key][keys[index]]) {
		build_seq(keys, index + 1, keys[index], curr_path + path + "A", result, paths);
    }

}


uint64_t shortest_seq(std::string keys, uint64_t depth, std::unordered_map<std::pair<std::string, uint64_t>, uint64_t, pair_hash>& cache, std::map<char, std::map<char, std::vector<std::string>>> paths) {
    if (depth == 0) { return keys.length(); }
	if (cache.find({ keys, depth }) != cache.end()) { return cache[{keys, depth}]; }
    auto split_on_A = [](const std::string& str) -> std::vector<std::string> {
        std::vector<std::string> result;
        std::string temp;

        for (char c : str) {
            temp += c;
            if (c == 'A') {
                result.push_back(temp);
                temp.clear();
            }
        }
        if (!temp.empty()) {
            result.push_back(temp);
        }

        return result;
    };
	auto key_parts = split_on_A(keys);
    uint64_t total = 0;
    for (const auto& part : key_parts) {
        std::vector<std::string> result;
		build_seq(part, 0, 'A', "", result, paths);
        uint64_t min = UINT64_MAX;
		for (const auto& r : result) {
            uint64_t len = shortest_seq(r, depth - 1, cache, paths);
            min = std::min(min, len);
			if (cache.find({ keys, depth }) == cache.end() || len < cache[{keys, depth}]) {
				cache[{keys, depth}] = len;
			}

		}
        total += min;
    }
	cache[{keys, depth}] = total;
	return total;
}

uint64_t solve(std::vector<std::string> input, std::map<char, std::map<char, std::vector<std::string>>> paths, uint64_t max_depth) {
	std::unordered_map<std::pair<std::string, uint64_t>, uint64_t, pair_hash> cache;
    uint64_t sum = 0;
    for (const auto& key : input) {
        std::vector<std::string> out;
		build_seq(key, 0, 'A', "", out, paths);
        uint64_t min = UINT64_MAX;
		for (const auto& o : out) {
            uint64_t len = shortest_seq(o, max_depth, cache, paths);
			min = std::min(min, len);
		}
		auto [m, n] = ctre::match<R"([a-zA-Z]*(\d+)[a-zA-Z]+)">(key);
        sum +=  min * n.to_number();
    }
    return sum;
}

int main() {

    INITIALIZE_AOC_TIMERS();

    default_timer.begin(0);

	std::vector<std::string> input = aoc_utils::read_lines("input.txt");

	default_timer.end(0);

	default_timer.begin(1);
	
    uint64_t p1 = 0, p2 = 0;


	std::map<char, std::map<char, std::vector<std::string>> > paths;
	std::vector<char> keypad_chars{ '7', '8', '9', '4', '5', '6', '1', '2', '3', '0', 'A' };
	for (const auto& c : keypad_chars) {
		for (const auto& c2 : keypad_chars) {
			paths[c][c2] = find_keypad_paths(keypad, c, c2);
		}
	}

	std::vector<char> dirpad_chars{ 'A', '>', 'v', '<', '^' };
	for (const auto& c : dirpad_chars) {
		for (const auto& c2 : dirpad_chars) {
			paths[c][c2] = find_keypad_paths(dirpad, c, c2);
		}
	}


	p1 = solve(input, paths, 2);

	default_timer.end(1);


	default_timer.begin(2);
    p2 = solve(input, paths, 25);
	default_timer.end(2);

	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

    default_timer.display_all();

}

//Part 1: 237342
//Part 2 : 294585598101704
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 210 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 44241.1 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 385530.4 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 448
//Ticks : 4481711
//TotalDays : 5.18716550925926E-06
//TotalHours : 0.000124491972222222
//TotalMinutes : 0.00746951833333333
//TotalSeconds : 0.4481711
//TotalMilliseconds : 448.1711