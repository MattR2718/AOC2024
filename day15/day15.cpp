#include "../utils/utils.h"

enum DIR {
	UP, RIGHT, DOWN, LEFT
};

void move(std::vector<std::string>& grid, int& x, int& y, int dir) {
	switch (dir) {
	case UP: {
		int ny = y - 1;
		while(grid[ny][x] == 'O') {
			--ny;
		}
		if (grid[ny][x] == '#') { break; }

		for(int j = ny; j < y; ++j) {
			grid[j][x] = grid[j + 1][x];
		}

		grid[y][x] = '.';
		y--;

		break;
	}
	case RIGHT: {
		int nx = x + 1;
		while(grid[y][nx] == 'O') {
			++nx;
		}
		if (grid[y][nx] == '#') { break; }

		for(int i = nx; i > x; --i) {
			grid[y][i] = grid[y][i - 1];
		}

		grid[y][x] = '.';
		x++;

		break;
	}
	case DOWN: { 
		int ny = y + 1;
		while(grid[ny][x] == 'O') {
			++ny;
		}
		if (grid[ny][x] == '#') { break; }

		for(int j = ny; j > y; --j) {
			grid[j][x] = grid[j - 1][x];
		}

		grid[y][x] = '.';
		y++;

		break;
	}
	case LEFT: { 
		int nx = x - 1;
		while(grid[y][nx] == 'O') {
			--nx;
		}
		if (grid[y][nx] == '#') { break; }

		for(int i = nx; i < x; ++i) {
			grid[y][i] = grid[y][i + 1];
		}		
		
		grid[y][x] = '.';
		x--;

		break;
	}
	}
}


bool move_dir(std::vector<std::string>& grid, const int xl, const int xr, const int y, const int dy, std::vector<std::pair<int, int>>& xly) {

	xly.push_back(std::pair<int, int>{ xl, y });

	if (grid[y + dy][xl] == '#' || grid[y + dy][xr] == '#') { return false; }
	if(grid[y + dy][xl] == '.' && grid[y + dy][xr] == '.') { return true; }

	if (grid[y + dy][xl] == '[' && grid[y + dy][xr] == ']') { return move_dir(grid, xl, xr, y + dy, dy, xly); }
	if (grid[y + dy][xl] == ']' && grid[y + dy][xr] == '[') { return move_dir(grid, xl - 1, xl, y + dy, dy, xly) && move_dir(grid, xr, xr + 1, y + dy, dy, xly); }
	if (grid[y + dy][xl] == ']' && grid[y + dy][xr] == '.') { return move_dir(grid, xl - 1, xl, y + dy, dy, xly); }
	if (grid[y + dy][xl] == '.' && grid[y + dy][xr] == '[') { return move_dir(grid, xr, xr + 1, y + dy, dy, xly); }

	std::cout << "NOT FOUND IN MOVEDIR: \n";
	std::cout<<"XL: "<<xl<<" XR: "<<xr<<" Y: "<<y<<" DY: "<<dy<<'\n';
	std::cout<<"GRIDXLDY: "<<grid[y + dy][xl]<<" GRIDXRDY: "<<grid[y + dy][xr]<<'\n';
	std::cin.get();
	return false;



	/*if(grid[y + dy][xl] == '#' || grid[y + dy][xr] == '#') { return false; }
	xly.emplace_back(std::pair<int, int>{ xl, y });
	if (grid[y + dy][xl] == '[' && grid[y + dy][xr] == ']') { return move_dir(grid, xl, xr, y + dy, dy, xly); }
	if (grid[y + dy][xl] == ']' && grid[y + dy][xr] == '[') { return move_dir(grid, xl - 1, xl, y + dy, dy, xly) && move_dir(grid, xr, xr + 1, y + dy, dy, xly); }
	if (grid[y + dy][xl] == ']') { return move_dir(grid, xl - 1, xl , y + dy, dy, xly); }
	if (grid[y + dy][xr] == '[') { return move_dir(grid, xr, xr + 1, y + dy, dy, xly); }
	return false;*/
}


void move2(std::vector<std::string>& grid, int& x, int& y, int dir) {
	switch (dir) {
	case UP: {
		std::vector<std::pair<int, int>> xly;
		if (grid[y - 1][x] == '#') { break; }
		if (grid[y - 1][x] == '.') {
			grid[y][x] = '.';
			grid[y - 1][x] = '@';
			y--; break; }

		bool mu = false;

		if(grid[y - 1][x] == '[') { 
			mu = move_dir(grid, x, x + 1, y - 1, -1, xly);
		}else if(grid[y - 1][x] == ']') {
			mu = move_dir(grid, x - 1, x, y - 1, -1, xly);
		}

		if (mu) {
			std::sort(xly.begin(), xly.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
				return a.second < b.second;
				});
			for(const auto& [xl, yv] : xly) {
				
				grid[yv][xl] = '.';
				grid[yv][xl + 1] = '.';

				grid[yv - 1][xl] = '[';
				grid[yv - 1][xl + 1] = ']';
				
				//grid[yv - 1][xl] = grid[xl][yv];
				//grid[b.second - 1][b.first + 1] = grid[b.second][b.first + 1];
				//grid[b.second][b.first] = '.';
				//grid[b.second][b.first + 1] = '.';
			}
			grid[y][x] = '.';
			grid[y - 1][x] = '@';
			y--;

		}
		break;
	}
	case RIGHT: {
		int nx = x + 1;
		while (grid[y][nx] == '[' || grid[y][nx] == ']') {
			++nx;
		}
		if (grid[y][nx] == '#') { break; }

		for (int i = nx; i > x; --i) {
			grid[y][i] = grid[y][i - 1];
		}

		grid[y][x] = '.';
		x++;

		break;
	}
	case DOWN: {

		std::vector<std::pair<int, int>> xly;
		if (grid[y + 1][x] == '#') { break; }
		if (grid[y + 1][x] == '.') {
			grid[y][x] = '.';
			grid[y + 1][x] = '@';
			y++; break;
		}

		bool mu = false;

		if (grid[y + 1][x] == '[') {
			mu = move_dir(grid, x, x + 1, y + 1, 1, xly);
		}
		else if (grid[y + 1][x] == ']') {
			mu = move_dir(grid, x - 1, x, y + 1, 1, xly);
		}

		if (mu) {
			std::sort(xly.begin(), xly.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
				return a.second > b.second;
				});
			for (const auto& [xl, yv] : xly) {

				grid[yv][xl] = '.';
				grid[yv][xl + 1] = '.';

				grid[yv + 1][xl] = '[';
				grid[yv + 1][xl + 1] = ']';

				/*grid[b.second + 1][b.first] = grid[b.second][b.first];
				grid[b.second + 1][b.first + 1] = grid[b.second][b.first + 1];
				grid[b.second][b.first] = '.';
				grid[b.second][b.first + 1] = '.';*/
			}
			grid[y][x] = '.';
			grid[y + 1][x] = '@';
			y++;

		}
		break;
	}
	case LEFT: {
		int nx = x - 1;
		while (grid[y][nx] == '[' || grid[y][nx] == ']') {
			--nx;
		}
		if (grid[y][nx] == '#') { break; }

		for (int i = nx; i < x; ++i) {
			grid[y][i] = grid[y][i + 1];
		}

		grid[y][x] = '.';
		x--;

		break;
	}
	}
}






void print_grid(const std::vector<std::string>& grid) {
	for(const auto& s : grid) {
		std::cout << s << '\n';
	}
	std::cout << '\n';
}

void print_grid_dir(const std::vector<std::string>& grid, int dir) {
	for (const auto& s : grid) {
		for (const char c : s) {
			if (c == '[' || c == ']') {
				fmt::print(fmt::fg(fmt::rgb(139, 69, 19)), "{}", c);
			}
			else if (c == '@') {
				switch (dir) {
				case UP: { fmt::print(fmt::fg(fmt::color::green), "^"); break; }
				case RIGHT: { fmt::print(fmt::fg(fmt::color::green), ">"); break; }
				case DOWN: { fmt::print(fmt::fg(fmt::color::green), "v"); break; }
				case LEFT: { fmt::print(fmt::fg(fmt::color::green), "<"); break; }
				}
			}

			else if (c == '#') {
				fmt::print(fmt::fg(fmt::color::gray), "#");
			}
			else {
				fmt::print(" ");
			}
		}
		std::cout<<'\n';
	}
	std::cout<<'\n';
}


int main() {

	INITIALIZE_AOC_TIMERS();

	default_timer.begin(0);

	std::vector<std::string> grid;
	std::vector<uint8_t> ins;
	bool ins_in = false;

	aoc_utils::read_lines_mmap_apply<int>("input.txt", [&grid, &ins, &ins_in](const std::string& s) {
		
		if (s.empty()) {
			ins_in = true;
			return 0;
		}

		if (!ins_in) {
			grid.emplace_back(s);
		}
		else {
			for (const char c : s) {
				switch (c) {
				case '^': { ins.emplace_back(UP); break; }
				case '>': { ins.emplace_back(RIGHT); break; }
				case 'v': { ins.emplace_back(DOWN); break; }
				case '<': { ins.emplace_back(LEFT); break; }
				}
			}
		}

		return 0;
		});

	// Part 1
	std::pair<int, int> pos{ 0, 0 };
	for(int j = 0; j < grid.size(); ++j) {
		for(int i = 0; i < grid[j].size(); ++i) {
			if (grid[j][i] == '@') {
				pos = { i, j };
				break;
			}
		}
	}

	// Part 2
	std::vector<std::string> grid2;
	for (const auto& s : grid) {
		grid2.push_back("");
		for (const char c : s) {
			if (c == '#') {
				grid2.back().push_back('#');
				grid2.back().push_back('#');
			}
			else if (c == '@') {
				grid2.back().push_back('@');
				grid2.back().push_back('.');
			}
			else if (c == 'O') {
				grid2.back().push_back('[');
				grid2.back().push_back(']');
			}
			else {
				grid2.back().push_back('.');
				grid2.back().push_back('.');
			}

		}
	}

	std::pair<int, int> pos2{ 0, 0 };
	for (int j = 0; j < grid2.size(); ++j) {
		for (int i = 0; i < grid2[j].size(); ++i) {
			if (grid2[j][i] == '@') {
				pos2 = { i, j };
				break;
			}
		}
	}

	default_timer.end(0);

	default_timer.begin(1);

	int p1 = 0;
	int p2 = 0;

	for (const auto i : ins) {
		move(grid, pos.first, pos.second, i);
	}

	for(int j = 0; j < grid.size(); ++j) {
		for(int i = 0; i < grid[j].size(); ++i) {
			if (grid[j][i] == 'O') {
				p1+= j * 100 + i;
			}
		}
	}

	default_timer.end(1);

	default_timer.begin(2);



	print_grid(grid2);
	std::cout<<"POS: "<<pos2.first<<" "<<pos2.second<<'\n';

	for (const auto i : ins) {

		/*switch (i) {
		case UP: { std::cout << "UP\n"; break; }
		case RIGHT: { std::cout << "RIGHT\n"; break; }
		case DOWN: { std::cout << "DOWN\n"; break; }
		case LEFT: { std::cout << "LEFT\n"; break; }
		}*/
		move2(grid2, pos2.first, pos2.second, i);
		//print_grid_dir(grid2, i);
		//std::cin.get();
	}

	//grid2 = { "##########", "##...[]...", "##........" };

	for (int j = 0; j < grid2.size(); ++j) {
		for (int i = 0; i < grid2[j].size(); ++i) {
			if (grid2[j][i] == '[') {
				//fmt::print(fmt::fg(fmt::color::green), "[");
				
				p2 += j * 100 + i;
			}
			else {
				//fmt::print(fmt::fg(fmt::color::dim_gray), "{}", grid2[j][i]);
			}
			std::cout << grid2[j][i];
		}
		fmt::println("");
	}
	


	default_timer.end(2);


	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

	default_timer.display_all();

}

// 1459391 high