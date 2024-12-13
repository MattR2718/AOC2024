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

//bool is_valid(int pos, int width, int height) {
//	int x = pos % width;
//	int y = pos / width;
//    return !(x < 0 || x >= width || y < 0 || y >= height);
//}
//
//bool is_valid(int x, int y, int width, int height) {
//	return !(x < 0 || x >= width || y < 0 || y >= height);
//}
//
//
//int find_edges(const std::string_view& sv, int width, int height, char c) {
//	int n = 0;
//
//	for (int i = 0; i < width; i++) {
//        for (int j = 0; j < height; j++) {
//            int pos = j * width + i;
//
//			std::cout << "Checking " << i << ", " << j << '\n';
//
//
//            if (is_valid(pos, width, height) && sv[pos] == c && 
//               (!is_valid(i, j + 1, width, height) || is_valid(i, j + 1, width, height) && sv[i + (j + 1) * width] != c) && 
//               (!is_valid(i - 1, j, width, height) || !is_valid(i + 1, j, width, height) || is_valid(i - 1, j, width, height) && sv[(i - 1) + j * width] != c || is_valid(i + 1, j, width, height) && sv[(i + 1) + j * width] != c)
//            ) {
//                n++;
//                std::cout << "Found edge, down vert\n";
//            }
//
//
//            if (is_valid(pos, width, height) && sv[pos] == c &&
//                (!is_valid(i, j - 1, width, height) || is_valid(i, j - 1, width, height) && sv[i + (j - 1) * width] != c) &&
//                (!is_valid(i - 1, j, width, height) || !is_valid(i + 1, j, width, height) || is_valid(i - 1, j, width, height) && sv[(i - 1) + j * width] != c || is_valid(i + 1, j, width, height) && sv[(i + 1) + j * width] != c)
//            ) {
//                n++;
//				std::cout << "Found edge, up vert\n";
//            }
//
//        }
//	}
//
//    for (int j = 0; j < height; j++) {
//        for (int i = 0; i < width; i++) {
//            int pos = j * width + i;
//            std::cout << "Checking " << i << ", " << j << '\n';
//
//            if (is_valid(pos, width, height) && sv[pos] == c &&
//                (!is_valid(i - 1, j, width, height) || is_valid(i - 1, j, width, height) && sv[i - 1 + (j) * width] != c) &&
//                (!is_valid(i, j - 1, width, height) || !is_valid(i, j + 1, width, height) || is_valid(i, j - 1, width, height) && sv[i + (j - 1) * width] != c || is_valid(i, j + 1, width, height) && sv[i + (j + 1) * width] != c)
//            ) {
//                n++;
//				std::cout << "Found edge, left hor\n";
//            }
//
//            if (is_valid(pos, width, height) && sv[pos] == c &&
//                (!is_valid(i + 1, j, width, height) || is_valid(i + 1, j, width, height) && sv[i + 1 + (j)*width] != c) &&
//                (!is_valid(i, j - 1, width, height) || !is_valid(i, j + 1, width, height) || is_valid(i, j - 1, width, height) && sv[i + (j - 1) * width] != c || is_valid(i, j + 1, width, height) && sv[i + (j + 1) * width] != c)
//            ) {
//                n++;
//				std::cout << "Found edge, right hor\n";
//            }
//        }
//    }
//
//	return n;
//
//}





struct three {
    char x, y, z;

    bool operator==(const three& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};


// Get sets of 2 rows/cols
// create tuples of elements x, y, z
// Delete duplicate elements
// Check how many elements are x, char or char, x
//int s2(const std::vector<std::string>& l) {
//
//    std::map<char, int> m;
//
//    
//    for (int i = 0; i < l.size() - 3; i++) {
//        std::vector<three> collection;
//		for (int j = 0; j < l[i].length(); j++) {
//			collection.push_back({ l[i][j], l[i + 1][j], l[i + 2][j] });
//		}
//
//		std::cout << "--------------------------" << '\n';
//		/*for (auto& [x, y, z] : collection) {
//			std::cout << x << y << z << '\n';
//		}
//
//        std::cout << "\n\n";*/
//
//        auto last = std::unique(collection.begin(), collection.end());
//
//        collection.erase(last, collection.end());
//
//        for (auto& [x, y, z] : collection) {
//            std::cout << x << y << z << '\n';
//        }
//
//        std::cout << "--------------------------" << '\n';
//
//    }
//    
//    return 0;
//}
//
//




















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


//void rename_region(std::vector<std::string>& map, int startRow, int startCol, char newChar, char originalChar) {
//    int rows = map.size();
//    int cols = map[0].size();
//    std::vector<std::pair<int, int>> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };
//
//    std::queue<std::pair<int, int>> q;
//    q.push({ startRow, startCol });
//    map[startRow][startCol] = newChar;
//
//    while (!q.empty()) {
//        auto [r, c] = q.front();
//        q.pop();
//
//        for (auto [dr, dc] : directions) {
//            int nr = r + dr;
//            int nc = c + dc;
//
//            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && map[nr][nc] == originalChar) {
//                map[nr][nc] = newChar;
//                q.push({ nr, nc });
//            }
//        }
//    }
//}
//
//
//
//void rename_regions(std::vector<std::string>& map) {
//    char nextChar = 'A';
//    for (int r = 0; r < map.size(); ++r) {
//        for (int c = 0; c < map[r].size(); ++c) {
//            //if (isalpha(map[r][c]) && map[r][c] < 'A') {
//                char originalChar = map[r][c];
//                rename_region(map, r, c, nextChar++, originalChar); // char is passed
//            //}
//        }
//    }
//}



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

                // Ensure we don’t run out of labels
                /*if (nextLabel > 'Z') {
                    std::cerr << "Error: Ran out of unique labels!\n";
                    return;
                }*/
            }
        }
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

    relabelGrid(l);
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



	for (const auto& s : big) {
		std::cout << s << '\n';
	}


    std::cout << "=========================================\n";






	//rename_regions(input, width, input.length() / width);

    for (int i = 'A'; i <= 'Z'; i++) {
		int c = solve2(big, i);
        int a = calculate_area(newa, width, input.length() / width, i);
		std::cout << "Char: " << (char)i << " Count: " << c << " Area: " << a << '\n';

        p2 += c * a;
    }

    for (int i = 0; i < input.length(); i++) {
        if (i % width == 0) std::cout << '\n';
        std::cout << newa[i];
    }
    std::cout << '\n';


	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';


}



// 8727056 high
// 8440656 high