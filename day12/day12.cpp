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

            // Check all neighbors
            for (auto& [dx, dy] : aoc_utils::cardinal_directions) {
                int nx = i % width + dx;
                int ny = i / width + dy;

                // Count perimeter for out of bounds
                if (nx < 0 || nx >= width || ny < 0 || ny >= (sv.length() / width)) {
                    perimeter++;
                    continue;
                }

                // Check neighbor cell
                int npos = ny * width + nx;
                if (sv[npos] != sv[pos]) {
                    // Different plant type contributes to perimeter
                    perimeter++;
                }
                else {
                    // Same plant type - add to queue if not visited
                    if (!visited.contains(npos)) {
                        q.push(npos);
                    }
                }
            }
        }
    }

    std::cout << sv[pos] << ": Area{" << area << "} Perimeter{" << perimeter << "}\n";
    return perimeter * area;
}



struct Edge {
    int x, y;
    bool is_horizontal;

    // For storing in set
    bool operator<(const Edge& other) const {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return is_horizontal < other.is_horizontal;
    }
};

int find_edges(const std::string& grid, int width, int height, char c) {
    std::set<Edge> edges;

    // Look at each 2x2 grid of cells
    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
            // Get the four cells in the 2x2 grid
            char top_left = grid[y * width + x];
            char top_right = grid[y * width + x + 1];
            char bottom_left = grid[(y + 1) * width + x];
            char bottom_right = grid[(y + 1) * width + x + 1];

            // Check if our target character is present
            bool has_target = (top_left == c || top_right == c ||
                bottom_left == c || bottom_right == c);

            if (!has_target) continue;

            // Check horizontal edges
            if ((top_left == c) != (bottom_left == c)) {
                edges.insert({ x, y + 1, true });
            }
            if ((top_right == c) != (bottom_right == c)) {
                edges.insert({ x + 1, y + 1, true });
            }

            // Check vertical edges
            if ((top_left == c) != (top_right == c)) {
                edges.insert({ x + 1, y, false });
            }
            if ((bottom_left == c) != (bottom_right == c)) {
                edges.insert({ x + 1, y + 1, false });
            }
        }
    }

    // Check edges along the right side of the grid
    for (int y = 0; y < height - 1; y++) {
        char top = grid[y * width + (width - 1)];
        char bottom = grid[(y + 1) * width + (width - 1)];
        if ((top == c) != (bottom == c)) {
            edges.insert({ width, y + 1, true });
        }
    }

    // Check edges along the bottom of the grid
    for (int x = 0; x < width - 1; x++) {
        char left = grid[(height - 1) * width + x];
        char right = grid[(height - 1) * width + x + 1];
        if ((left == c) != (right == c)) {
            edges.insert({ x + 1, height, false });
        }
    }

    // Each unique edge in the set represents one side
    return edges.size();
}

int calculate_area(const std::string& grid, int width, int height, char c) {
    int area = 0;
    for (int i = 0; i < width * height; i++) {
        if (grid[i] == c) area++;
    }
    return area;
}

int calculate_region_price(const std::string& grid, int width, int height, char c) {
    int sides = find_edges(grid, width, height, c);
    int area = calculate_area(grid, width, height, c);
    return area * sides;
}



bool check_corner(const std::vector<std::string>& s, int x, int y, char c) {
    int cor = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (s[y + i][x + j] == c) cor++;
        }
    }

    return cor == 3 || cor == 1;
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


int solve2(const std::vector<std::string>& l, char c) {
    int cor = 0;
	for (int i = 0; i < l.size() - 2; i++) {
		for (int j = 0; j < l[i].length() - 2; j++) {
			if (check_corner(l, j, i, c)) cor++;
		}
	}

    return cor;
}


void renameRegion(std::vector<std::string>& map, int startRow, int startCol, char newChar, char originalChar) {
    int rows = map.size();
    int cols = map[0].size();
    std::vector<std::pair<int, int>> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    std::queue<std::pair<int, int>> q;
    q.push({ startRow, startCol });
    map[startRow][startCol] = newChar;

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (auto [dr, dc] : directions) {
            int nr = r + dr;
            int nc = c + dc;

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && map[nr][nc] == originalChar) {
                map[nr][nc] = newChar;
                q.push({ nr, nc });
            }
        }
    }
}

void renameRegions(std::vector<std::string>& map) {
    char nextChar = 'A';
    for (int r = 0; r < map.size(); ++r) {
        for (int c = 0; c < map[r].size(); ++c) {
            if (isalpha(map[r][c])) {
                char originalChar = map[r][c];
                renameRegion(map, r, c, nextChar++, originalChar);
            }
        }
    }
}

std::vector<std::string> convertToMap(const std::string& input, int width, int height) {
    std::vector<std::string> map;
    for (int i = 0; i < height; ++i) {
        map.push_back(input.substr(i * width, width));
    }
    return map;
}

std::string mapToString(const std::vector<std::string>& map) {
    std::string result;
    for (const auto& row : map) {
        result += row;
    }
    return result;
}

std::string processMap(const std::string& input, int width, int height) {
    std::vector<std::string> map = convertToMap(input, width, height);
    renameRegions(map);
    return mapToString(map);
}



std::vector<std::pair<int, int>> make_bigger_shape(const std::string_view& sv, int width, int height, char c) {
	std::vector<std::pair<int, int>> new_shape;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {

            if (sv[j * width + i] == c) {
                for (int k = 0; k < 3; k++) {
                    for (int l = 0; l < 3; l++) {
                        new_shape.push_back({ i + k, j + l });
                    }
                }
            }
			
		}
	}
	return new_shape;
}


void relabelRegion(std::vector<std::string>& grid, int x, int y, char oldChar, char newChar) {
    int rows = grid.size();
    int cols = grid[0].size();

    // Directions for moving up, down, left, right
    std::vector<std::pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    // Queue for BFS
    std::queue<std::pair<int, int>> q;
    q.push({ x, y });
    grid[x][y] = newChar;

    while (!q.empty()) {
        auto [curX, curY] = q.front();
        q.pop();

        for (auto [dx, dy] : directions) {
            int newX = curX + dx;
            int newY = curY + dy;

            if (newX >= 0 && newX < rows && newY >= 0 && newY < cols && grid[newX][newY] == oldChar) {
                grid[newX][newY] = newChar;
                q.push({ newX, newY });
            }
        }
    }
}

void relabelGrid(std::vector<std::string>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    char nextLabel = 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] != '.' && isupper(grid[i][j])) {
                char oldChar = grid[i][j];
                relabelRegion(grid, i, j, oldChar, nextLabel);
                nextLabel++;

            }
        }
    }
}

















using namespace std;

// Directions for 4 neighbors (Up, Down, Left, Right)
int dx[] = { -1, 1, 0, 0 };
int dy[] = { 0, 0, -1, 1 };

// Check if a cell is within grid bounds
bool isValid(int x, int y, int n, int m) {
    return (x >= 0 && x < n && y >= 0 && y < m);
}

// BFS to explore connected components of the same character
void bfs(int x, int y, char target, vector<string>& grid, vector<vector<bool>>& visited, vector<pair<int, int>>& component) {
    queue<pair<int, int>> q;
    q.push({ x, y });
    visited[x][y] = true;

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();
        component.push_back({ cx, cy });

        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i], ny = cy + dy[i];
            if (isValid(nx, ny, grid.size(), grid[0].size()) && !visited[nx][ny] && grid[nx][ny] == target) {
                visited[nx][ny] = true;
                q.push({ nx, ny });
            }
        }
    }
}

// Split the grid into two rectangular parts
void splitGrid(vector<string>& grid) {
    int n = grid.size(), m = grid[0].size();

    // Step 1: Find connected components
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<vector<pair<int, int>>> components;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (!visited[i][j]) {
                vector<pair<int, int>> component;
                bfs(i, j, grid[i][j], grid, visited, component);
                components.push_back(component);
            }
        }
    }

    // Step 2: Find a way to split the grid
    // For simplicity, we'll split the grid vertically into two equal parts
    int split_col = m / 2;

    // Step 3: Apply the split
    vector<string> grid1(n, string(split_col, '\0'));
    vector<string> grid2(n, string(m - split_col, '\0'));

    // Copy components to the two grids
    for (const auto& component : components) {
        bool inLeft = false, inRight = false;
        for (const auto& [x, y] : component) {
            if (y < split_col) inLeft = true;
            else inRight = true;
        }

        if (inLeft && inRight) {
            // If the component is split, place it entirely in one grid (choose left or right arbitrarily)
            // Here we choose the left grid for simplicity
            for (const auto& [x, y] : component) {
                grid1[x][y] = grid[x][y];
            }
        }
        else if (inLeft) {
            // If the component is entirely in the left part
            for (const auto& [x, y] : component) {
                grid1[x][y] = grid[x][y];
            }
        }
        else {
            // If the component is entirely in the right part
            for (const auto& [x, y] : component) {
                grid2[x][y - split_col] = grid[x][y];
            }
        }
    }

    // Step 4: Print the two grids
    cout << "Grid 1:" << endl;
    for (int i = 0; i < n; ++i) {
        cout << grid1[i] << endl;
    }

    cout << "\nGrid 2:" << endl;
    for (int i = 0; i < n; ++i) {
        cout << grid2[i] << endl;
    }
}
















int main() {	
	auto l = aoc_utils::read_lines("input.txt");
	std::string input = "";
	int width = l[0].length();
	for (auto& s : l) {
		input += s;
	}
	
	std::cout << width << '\n';
	std::cout << input.length() << '\n';
	std::cout << input << '\n';



	for (int i = 0; i < input.length(); i++) {
		if (i % width == 0) std::cout << '\n';
		std::cout << input[i];
	}

	std::cout << "\n\n";
	
	int p1 = 0, p2 = 0;

	std::set<int> visited;
	for (int i = 0; i < input.length(); i++) {
		if (visited.contains(i)) continue;
		p1 += ffill(input, i, visited, width);

	}

	// Part 2

    //relabelGrid(l);
    std::string newa = "";
	for (const auto& s : l) {
		newa += s;
	}

    std::string s = "";
	for (int i = 0; i < width + 6; i++) {
		s += ".";
	}

    std::vector<std::string> l2 = { s, s, s };
	for (const auto& line : l) {
		l2.push_back("..." + line + "...");
	}
    l2.push_back(s);
    l2.push_back(s);
    l2.push_back(s);

	for (auto& line : l2) {
		std::cout << line << '\n';
	}


	auto big = make_bigger(l2);


    std::cout << "=========================================\n";
	std::cout << "1\n";


	for (const auto& s : big) {
		std::cout << s << '\n';
	}


    std::cout << "=========================================\n";
    std::cout << "2\n";



	splitGrid(l);



    for (int i = 'A'; i <= 'Z'; i++) {
		int c = solve2(big, i);
        int a = calculate_area(newa, width, input.length() / width, i);
		//std::cout << "Char: " << (char)i << " Count: " << c << " Area: " << a << '\n';

        p2 += c * a;
    }

    /*for (int i = 0; i < input.length(); i++) {
        if (i % width == 0) std::cout << '\n';
        std::cout << newa[i];
    }
    std::cout << '\n';*/


	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';


}



// 8727056 high
// 8440656 high