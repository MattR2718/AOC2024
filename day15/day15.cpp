#include "../utils/utils.h"

enum DIR {
	UP, RIGHT, DOWN, LEFT
};

void move(std::vector<std::string>& grid, int& x, int& y, int dx, int dy) {

	int nx = x + dx;
	int ny = y + dy;

	while(grid[ny][nx] == 'O') {
		nx += dx;
		ny += dy;
	}
	if(grid[ny][nx] == '#') { return; }

	while(nx != x || ny != y) {
		grid[ny][nx] = grid[ny - dy][nx - dx];
		nx -= dx;
		ny -= dy;
	}
	y += dy;
	x += dx;

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

}

void move2(std::vector<std::string>& grid, int& x, int& y, int dx, int dy) {
	// UP/DOWN
	if (dy != 0) {
		std::vector<std::pair<int, int>> xly;
		if (grid[y + dy][x] == '#') { return; }
		if (grid[y + dy][x] == '.') {
			grid[y][x] = '.';
			grid[y + dy][x] = '@';
			y += dy;
			return;
		}

		bool mu = false;

		if (grid[y + dy][x] == '[') {
			mu = move_dir(grid, x, x + 1, y + dy, dy, xly);
		}
		else if (grid[y + dy][x] == ']') {
			mu = move_dir(grid, x - 1, x, y + dy, dy, xly);
		}

		if (mu) {
			std::sort(xly.begin(), xly.end(), [dy](const std::pair<int, int>& a, const std::pair<int, int>& b) {
				return (dy == -1) ? a.second < b.second : a.second > b.second;
				});
			for (const auto& [xl, yv] : xly) {

				grid[yv][xl] = '.';
				grid[yv][xl + 1] = '.';

				grid[yv + dy][xl] = '[';
				grid[yv + dy][xl + 1] = ']';

			}
			grid[y][x] = '.';
			grid[y + dy][x] = '@';
			y += dy;

		}
	}
	else { // LEFT/RIGHT
		int nx = x + dx;
		int ny = y + dy;

		while (grid[y][nx] == '[' || grid[y][nx] == ']') {
			nx += dx;
			ny += dy;
		}
		if (grid[ny][nx] == '#') { return; }

		while (nx != x || ny != y) {
			grid[ny][nx] = grid[ny - dy][nx - dx];
			nx -= dx;
			ny -= dy;
		}
		grid[y][x] = '.';
		grid[y][x + dx] = '@';
		x += dx;
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

	// Part 1 pos
	std::pair<int, int> pos{ 0, 0 };
	for(int j = 0; j < grid.size(); ++j) {
		for(int i = 0; i < grid[j].size(); ++i) {
			if (grid[j][i] == '@') {
				pos = { i, j };
				break;
			}
		}
	}

	// Part 2 grid
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

	// Part 2 pos
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


	//
	//
	//
	//
	// Part 1

	default_timer.begin(1);

	int p1 = 0;
	int p2 = 0;

	constexpr int dirs[4][2] = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };
	for (const auto i : ins) {
		move(grid, pos.first, pos.second, dirs[i][0], dirs[i][1]);
	}

	for(int j = 0; j < grid.size(); ++j) {
		for(int i = 0; i < grid[j].size(); ++i) {
			if (grid[j][i] == 'O') {
				p1+= j * 100 + i;
			}
		}
	}

	default_timer.end(1);


	//
	//
	//
	// Part 2

	default_timer.begin(2);

	for (const auto i : ins) {
		move2(grid2, pos2.first, pos2.second, dirs[i][0], dirs[i][1]);
	}

	for (int j = 0; j < grid2.size(); ++j) {
		for (int i = 0; i < grid2[j].size(); ++i) {
			if (grid2[j][i] == '[') {
				p2 += j * 100 + i;
			}
		}
	}

	default_timer.end(2);


	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

	default_timer.display_all();

}

//Part 1: 1471826
//Part 2 : 1457703
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 849.5 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 452.1 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 1073.3 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 32
//Ticks : 320561
//TotalDays : 3.71019675925926E-07
//TotalHours : 8.90447222222222E-06
//TotalMinutes : 0.000534268333333333
//TotalSeconds : 0.0320561
//TotalMilliseconds : 32.0561