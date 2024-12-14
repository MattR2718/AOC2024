#include "../utils/utils.h"

#include <set>

int ffill(const std::string_view& sv, int pos, std::set<int>& visited, int width) {
    std::queue<int> q;
    q.push(pos);
    int area = 0, perimeter = 0;

    while (!q.empty()) {
        int i = q.front();
        q.pop();
        if (visited.contains(i)) continue;

        if (sv[i] == sv[pos]) {
            area++;
            visited.insert(i);

            for (auto& [dx, dy] : aoc_utils::cardinal_directions) {
                int nx = i % width + dx;
                int ny = i / width + dy;

                if (nx < 0 || nx >= width || ny < 0 || ny >= (sv.length() / width)) {
                    perimeter++;
                    continue;
                }

                int npos = ny * width + nx;
                if (sv[npos] != sv[pos]) {
                    perimeter++;
                }
                else {
                    if (!visited.contains(npos)) {
                        q.push(npos);
                    }
                }
            }
        }
    }

    //std::cout << sv[pos] << ": Area{" << area << "} Perimeter{" << perimeter << "}\n";
    return perimeter * area;
}

std::vector<std::string> make_bigger(const std::vector<std::string>& l) {
    std::vector<std::string> new_l;
    std::string s = "";
    for (int i = 0; i < l.size(); i++) {
        for (int j = 0; j < l[i].length(); j++) {
            s += l[i][j];
            s += l[i][j];
            s += l[i][j];
        }
        new_l.push_back(s);
        new_l.push_back(s);
        new_l.push_back(s);
        s = "";
    }
    return new_l;
}

std::vector<std::vector<std::pair<int, int>>> find_regions(const std::vector<std::string>& grid) {
    if (grid.empty() || grid[0].empty()) {
        return {};
    }

    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<std::pair<int, int>>> regions;

    const int dx[] = { -1, 0, 1, 0 };
    const int dy[] = { 0, 1, 0, -1 };

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!visited[i][j]) {
                std::vector<std::pair<int, int>> currentRegion;
                char currentChar = grid[i][j];

                std::queue<std::pair<int, int>> q;
                q.push({ i, j });
                visited[i][j] = true;
                currentRegion.push_back({ i, j });

                while (!q.empty()) {
                    auto [curr_i, curr_j] = q.front();
                    q.pop();

                    for (int dir = 0; dir < 4; ++dir) {
                        int new_i = curr_i + dx[dir];
                        int new_j = curr_j + dy[dir];

                        if (new_i >= 0 && new_i < rows &&
                            new_j >= 0 && new_j < cols &&
                            !visited[new_i][new_j] &&
                            grid[new_i][new_j] == currentChar) {

                            visited[new_i][new_j] = true;
                            q.push({ new_i, new_j });
                            currentRegion.push_back({ new_i, new_j });
                        }
                    }
                }

                regions.push_back(currentRegion);
            }
        }
    }

    return regions;
}

std::vector<std::string> normalize_region(const std::vector<std::pair<int, int>>& reg) {
    // Step 1: Find the bounding box of the region
    int min_x = std::numeric_limits<int>::max();
    int max_x = std::numeric_limits<int>::min();
    int min_y = std::numeric_limits<int>::max();
    int max_y = std::numeric_limits<int>::min();

    // Find the min/max x and y values in the region
    for (const auto& cell : reg) {
        min_x = std::min(min_x, cell.first);
        max_x = std::max(max_x, cell.first);
        min_y = std::min(min_y, cell.second);
        max_y = std::max(max_y, cell.second);
    }

    // Step 2: Create a grid with normalized size
    int width = max_x - min_x + 1 + 6;
    int height = max_y - min_y + 1 + 6;

    // Initialize the grid with '.'
    std::vector<std::string> grid(height, std::string(width, '.'));

    // Step 3: Set the cells in the region to '#'
    for (const auto& cell : reg) {
        int x = cell.first - min_x;  // Normalizing x
        int y = cell.second - min_y; // Normalizing y
        grid[y + 3][x + 3] = '#';  // Set the normalized position to '#'
    }

    return grid;
}

void print_grid(const std::vector<std::string>& grid) {
    for (const auto& row : grid) {
        std::cout << row << std::endl;
    }
}

int check_corner(const std::vector<std::string>& s, int x, int y) {
	// Handle mobius strip
    if (s[y][x] == '.' && s[y][x + 1] == '#' && s[y + 1][x] == '#' && s[y + 1][x + 1] == '.') return 2;
    if (s[y][x] == '#' && s[y][x + 1] == '.' && s[y + 1][x] == '.' && s[y + 1][x + 1] == '#') return 2;

    int cor = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (s[j + y][i + x] == '#') cor++;
        }
    }

    return cor == 3 || cor == 1;
}

int solve2(const std::vector<std::pair<int, int>>& reg) {
	std::vector<std::string> norm = normalize_region(reg);
    int c = 0;
    for(int j = 0; j < norm.size() - 2; j++){
		for (int i = 0; i < norm[j].size() - 2; i++) {
            c += check_corner(norm, i, j);
		}
    }

    return c * (reg.size() / 9);
}



int main() {

    INITIALIZE_AOC_TIMERS();

	default_timer.begin(0);

	auto l = aoc_utils::read_lines("input.txt");
	std::string input = "";
	int width = l[0].length();
	for (auto& s : l) {
		input += s;
	}

	default_timer.end(0);
	
	int p1 = 0, p2 = 0;

	// Part 1
	default_timer.begin(1);
	std::set<int> visited;
	for (int i = 0; i < input.length(); i++) {
		if (visited.contains(i)) continue;
		p1 += ffill(input, i, visited, width);
	}
	default_timer.end(1);

	// Part 2
	default_timer.begin(2);
	auto big = make_bigger(l);
    auto regions = find_regions(big);

    for (const auto& r : regions) {
        p2 += solve2(r);
    }

	default_timer.end(2);

	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

    default_timer.display_all();
}

//Part 1: 1377008
//Part 2 : 815788
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 265.4 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 8175.2 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 6992.7 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 34
//Ticks : 341466
//TotalDays : 3.95215277777778E-07
//TotalHours : 9.48516666666667E-06
//TotalMinutes : 0.00056911
//TotalSeconds : 0.0341466
//TotalMilliseconds : 34.1466