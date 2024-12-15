#include "../utils/utils.h"


#define test false

struct Robot {
	int x = 0, y = 0;
	int dx = 0, dy = 1;

	void print() const {
		fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::cyan), "Position: ");
		fmt::print(fmt::fg(fmt::color::green), "({}, {})\n", x, y);
		fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::cyan), "Speed: ");
		fmt::print(fmt::fg(fmt::color::yellow), "({}, {})\n", dx, dy);
	}

	void move(int width, int height) {
		x = (x + dx) % width;
		if (x < 0) x += width;

		y = (y + dy) % height;
		if (y < 0) y += height;
	}
};
int get_quad(int x, int y, int width, int height) {
	if (x == width / 2 || y == height / 2) {
		return -1;
	}

	if (x < width / 2) {
		if (y < height / 2) {
			return 0;
		}
		else {
			return 2;
		}
	}
	else {
		if (y < height / 2) {
			return 1;
		}
		else {
			return 3;
		}
	}
}



void draw_grid(const std::vector<Robot>& robots, int width, int height) {
	std::vector<std::vector<int>> grid(height, std::vector<int>(width, 0));

	for (const auto& robot : robots) {
		if (robot.x >= 0 && robot.x < width && robot.y >= 0 && robot.y < height) {
			grid[robot.y][robot.x]++;
		}
	}
	
	fmt::print("\n\n");

	for (int y = 0; y < height; ++y) {
		fmt::print(fmt::fg(fmt::color::gray), "|");
		for (int x = 0; x < width; ++x) {
			if (grid[y][x] == 0) {
				fmt::print(fmt::fg(fmt::color::white), ".");
			}
			else {
				fmt::print(fmt::fg(fmt::color::red) | fmt::emphasis::bold, "{}", grid[y][x]);
			}
		}
		fmt::print(fmt::fg(fmt::color::gray), "|\n");
	}

	fmt::print("\n\n");
}


void draw_grid_plain(const std::vector<Robot>& robots, int width, int height) {
	std::vector<std::vector<int>> grid(height, std::vector<int>(width, 0));

	for (const auto& robot : robots) {
		if (robot.x >= 0 && robot.x < width && robot.y >= 0 && robot.y < height) {
			grid[robot.y][robot.x]++;
		}
	}

	std::cout << "\n\n";

	for (int y = 0; y < height; ++y) {
		std::cout << "|";
		for (int x = 0; x < width; ++x) {
			if (grid[y][x] == 0) {
				std::cout << ".";
			}
			else {
				std::cout << grid[y][x];
			}
		}
		std::cout << "|\n";
	}

	std::cout << "\n\n";
}



bool isConnected(const Robot& a, const Robot& b) {
	for (const auto& dir : aoc_utils::all_directions) {
		if (a.x + dir.first == b.x && a.y + dir.second == b.y) {
			return true;
		}
	}
	return false;
}

int floodFill(std::vector<Robot>& robots, Robot& start, std::unordered_set<Robot*>& visited) {
	std::stack<Robot*> stack;
	stack.push(&start);
	visited.insert(&start);

	int size = 0;
	while (!stack.empty()) {
		Robot* current = stack.top();
		stack.pop();
		size++;

		for (auto& robot : robots) {
			if (visited.find(&robot) == visited.end() && isConnected(*current, robot)) {
				visited.insert(&robot);
				stack.push(&robot);
			}
		}
	}

	return size;
}

int findLargestRegion(std::vector<Robot>& robots) {
	std::unordered_set<Robot*> visited;
	int largestRegion = 0;

	for (auto& robot : robots) {
		if (visited.find(&robot) == visited.end()) {
			int regionSize = floodFill(robots, robot, visited);
			largestRegion = std::max(largestRegion, regionSize);
		}
	}

	return largestRegion;
}


int main() {
	INITIALIZE_AOC_TIMERS();

	default_timer.begin(0);

#if test
	int width = 11, height = 7;
	std::vector<Robot> robots = aoc_utils::read_lines_mmap<Robot>("test.txt", [](const std::string& line) {
#else
	int width = 101, height = 103;
	std::vector<Robot> robots = aoc_utils::read_lines_mmap<Robot>("input.txt", [](const std::string& line) {
#endif
		auto [m, x, y, dx, dy] = ctre::search<R"(p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+))">(line);

		return Robot{ x.to_number(), y.to_number(), dx.to_number(), dy.to_number() };
		});

	default_timer.end(0);

	default_timer.begin(1);

	int p1 = 1, p2 = 0;

	auto r1 = robots;

	for (int i = 0; i < 100; i++) {

		std::for_each(std::execution::par, r1.begin(), r1.end(), [width, height](Robot& r) { r.move(width, height); });

	}

	std::vector<std::map<std::pair<int, int>, int>> quads(4);

	for (const Robot& r : r1) {
		int q = get_quad(r.x, r.y, width, height);
		if (q >= 0) {
			quads[q][std::make_pair(r.x, r.y)]++;
		}
	}

	for (auto& m : quads) {
		int n = 0;
		for (auto& [k, v] : m) {
			n += v;
		}
		p1 *= n;
	}

	default_timer.end(1);

	default_timer.begin(2);

	std::vector<int> lrs;
	for (int i = 0; i < 8055; i++) {

		std::for_each(std::execution::par, robots.begin(), robots.end(), [width, height](Robot& r) { r.move(width, height); });

		lrs.push_back(findLargestRegion(robots));

	}

	int im = -1, m = INT_MIN;
	for(int i = 0; i < lrs.size(); i++) {
		if(m < lrs[i]) {
			m = lrs[i];
			im = i;
		}
	}

	p2 = im + 1;

	default_timer.end(2);

	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

	default_timer.display_all();

}


//Part 1: 219150360
//Part 2 : 8053
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 604.9 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 6608.3 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 21079500.8 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 21
//Milliseconds : 121
//Ticks : 211214724
//TotalDays : 0.000244461486111111
//TotalHours : 0.00586707566666667
//TotalMinutes : 0.35202454
//TotalSeconds : 21.1214724
//TotalMilliseconds : 21121.4724