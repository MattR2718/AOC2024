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
    {0, 1},
    {1, 0},
    {-1, 0},
    {0, -1}
        
};

struct State {
    int row, col;
    std::string path;
};


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

std::pair<int, int> find_key_position(char key, const std::vector<std::vector<char>>& keypad) {
    for (int r = 0; r < keypad.size(); r++) {
        for (int c = 0; c < keypad[r].size(); c++) {
            if (keypad[r][c] == key) {
                return { r, c };
            }
        }
    }
    return { -1, -1 };
}

std::string bfs(char start, char target, const std::vector<std::vector<char>>& keypad) {
    auto [sr, sc] = find_key_position(start, keypad);
    auto [tr, tc] = find_key_position(target, keypad);

    std::queue<State> q;
    q.push({ sr, sc, "" });

    std::vector<std::vector<bool>> visited(keypad.size(), std::vector<bool>(keypad[0].size(), false));
    visited[sr][sc] = true;

    while (!q.empty()) {
        State curr = q.front();
        q.pop();

        if (curr.row == tr && curr.col == tc) {
            return curr.path;
        }

        for (int i = 0; i < 4; i++) {
            int nr = curr.row + dirs[i] .first;
            int nc = curr.col + dirs[i].second;

            if (nr >= 0 && nr < keypad.size() && nc >= 0 && nc < keypad[nr].size() && keypad[nr][nc] != ' ' && !visited[nr][nc]) {
                visited[nr][nc] = true;
                q.push({ nr, nc, curr.path + e_t_c((DIRECTION)i)});
            }
        }
    }
    return "N/A";
}

std::vector<std::string> bfs_all_paths(char start, char target, const std::vector<std::vector<char>>& keypad) {
    auto [sr, sc] = find_key_position(start, keypad);
    auto [tr, tc] = find_key_position(target, keypad);

    std::queue<State> q;
    q.push({ sr, sc, "" });

    std::vector<std::string> paths;

    while (!q.empty()) {
        State curr = q.front();
        q.pop();

        if (curr.row == tr && curr.col == tc) {
            paths.push_back(curr.path);
            continue;  // Continue to explore other paths
        }

        for (int i = 0; i < 4; i++) {
            int nr = curr.row + dirs[i].first;
            int nc = curr.col + dirs[i].second;

            if (nr >= 0 && nr < keypad.size() && nc >= 0 && nc < keypad[nr].size() && keypad[nr][nc] != ' ') {
                q.push({ nr, nc, curr.path + e_t_c((DIRECTION)i) });
            }
        }
    }
    return paths;
}

std::unordered_map<char, std::unordered_map<char, std::string>> generate_paths_keypad(const std::vector<std::vector<char>>& keypad) {
    std::unordered_map<char, std::unordered_map<char, std::string>> paths;

    for (auto& row : keypad) {
        for (char key : row) {
            if (key != ' ') {
                for (auto& targetRow : keypad) {
                    for (char target : targetRow) {
                        if (target != ' ') {
                            paths[key][target] = bfs(key, target, keypad);
                        }
                    }
                }
            }
        }
    }
    return paths;
}






std::string optimize_segment(const std::string& segment, char from, char to, bool keypad) {
	std::string optimized = "";
    std::map<char, std::vector<char>> pref;
    if (keypad) {
		//pref['0'] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A' };
        return segment;

    }
    else {
        pref['^'] = { '<', 'v', '>' };
        pref['>'] = { '<', 'v', '^' };
        pref['v'] = { '<', '>', '^' };
        pref['<'] = { 'v', '^', '>' };

        auto sortByPreference = [&](const std::string& str) {
            std::string sortedStr = str;
            std::sort(sortedStr.begin(), sortedStr.end(), [&](char a, char b) {
                const auto& prefA = pref[a];
                const auto& prefB = pref[b];
                auto itA = std::find(prefA.begin(), prefA.end(), b);
                auto itB = std::find(prefB.begin(), prefB.end(), a);
                return (itA != prefA.end() && itB == prefB.end());
                });
            return sortedStr;
            };

        // Step 2: Split the string by 'A' and store the sections
        std::vector<std::string> parts;
        size_t lastPos = 0;

        for (size_t i = 0; i < segment.length(); ++i) {
            if (segment[i] == 'A') {
                // Push the section between 'A's and the 'A' itself
                if (i > lastPos) {
                    parts.push_back(segment.substr(lastPos, i - lastPos)); // Add part before 'A'
                }
                parts.push_back("A"); // Add the 'A'
                lastPos = i + 1;
            }
        }

        // If there's anything left after the last 'A', add it
        if (lastPos < segment.length()) {
            parts.push_back(segment.substr(lastPos)); // Add the remaining part
        }

        // Step 3: Sort the parts between 'A's
        for (size_t i = 0; i < parts.size(); ++i) {
            if (parts[i] != "A") {
                parts[i] = sortByPreference(parts[i]);
            }
        }

        // Step 4: Reassemble the string
        std::string result;
        for (const auto& part : parts) {
            result += part;
        }

		return result;

    }
    return segment;
}



















std::string get_path(const std::unordered_map<char, std::unordered_map<char, std::string>>& paths, char start, std::string output, bool keypad) {
    std::string path = "";
    for (char c : output) {
        //std::cout << c << '\n';
        std::string segment = paths.at(start).at(c);

		segment = optimize_segment(segment, start, c, keypad);
        
		//std::cout << "Adding: " << start << " -> " << c << " : " << segment << '\n';
        
        if (!segment.empty() && segment.back() == 'A') {
            path += segment;
        }
        else {
            path += segment + "A";
        }
        start = c;
    }
    return path;
}

int main() {

    INITIALIZE_AOC_TIMERS();

    default_timer.begin(0);

	std::vector<std::string> input = aoc_utils::read_lines("input.txt");

	for (const auto& line : input) {
		std::cout << line << '\n';
	}

    

	default_timer.end(0);

	default_timer.begin(1);
	
	int p1 = 0, p2 = 0;

    auto keypad_paths = generate_paths_keypad(keypad);

    for (auto& start : keypad_paths) {
        for (auto& end : start.second) {
            std::cout << "Path from " << start.first << " to " << end.first << ": " << end.second << "\n";
        }
    }

    std::cout << "\n\n\n";

	auto dirpad_paths = generate_paths_keypad(dirpad);

	for (auto& start : dirpad_paths) {
		for (auto& end : start.second) {
			std::cout << "Path from " << start.first << " to " << end.first << ": " << end.second << "\n";
		}
	}



    for (const auto& l : input) {
        std::cout << l << '\n';
        auto p1 = get_path(keypad_paths, 'A', l, true);
		std::cout << p1 << '\n';
		auto p2 = get_path(dirpad_paths, 'A', p1, false);
		std::cout << p2 << '\n';
    }


    std::cout << "PATH: " << get_path(dirpad_paths, 'A', get_path(dirpad_paths, 'A', get_path(keypad_paths, 'A', input[0], true), false), false) << '\n';

    char kp_s = 'A';
	char dp1_s = 'A';
	char dp2_s = 'A';
	for (const auto& line : input) {
        std::string path = get_path(dirpad_paths, dp2_s, get_path(dirpad_paths, dp1_s, get_path(keypad_paths, kp_s, line, true), false), false);
        auto [m, n] = ctre::match<R"([a-zA-Z]*(\d+)[a-zA-Z]+)">(line);

        std::cout << n.to_number() << " * " << path.length() << "                  " << path << '\n';

        p1 += n.to_number() * path.length();
    }





	default_timer.end(1);


	default_timer.begin(2);
	default_timer.end(2);

	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

    default_timer.display_all();

}

// 253278 high
// 243574 high












/*



v<<A    A -> <
>>^A    < -> A
<A      A -> ^
>A      ^ -> A
vA      A -> >
<^A     > -> ^
A
>A
<vA
A
A
>^A













*/