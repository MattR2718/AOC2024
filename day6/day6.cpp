#include "../utils/utils.h"

#include <set>

enum {
	UP = 0,
	RIGHT = 1,
	DOWN = 2,
	LEFT = 3
};

void move(const std::string_view& sv, int width, std::pair<int, int>& pos, int& dir, std::set<int>& visited, std::string& s) {
	switch (dir) {
	case UP:
		if (sv[pos.first + (pos.second - 1) * width] == '.' || sv[pos.first + (pos.second - 1) * width] == '^')
			pos.second--;
		else if (sv[pos.first + (pos.second - 1) * width] == '#') {
			dir = (dir + 1) % 4;
		}
		break;
	case RIGHT:
		if (sv[pos.first + 1 + pos.second * width] == '.' || sv[pos.first + 1 + pos.second * width] == '^')
			pos.first++;
		else if (sv[pos.first + 1 + pos.second * width] == '#') {
			dir = (dir + 1) % 4;
		}
		break;
	case DOWN:
		if (sv[pos.first + (pos.second + 1) * width] == '.' || sv[pos.first + (pos.second + 1) * width] == '^')
			pos.second++;
		else if (sv[pos.first + (pos.second + 1) * width] == '#') {
			dir = (dir + 1) % 4;
		}
		break;
	case LEFT:
		if (sv[pos.first - 1 + pos.second * width] == '.' || sv[pos.first - 1 + pos.second * width] == '^')
			pos.first--;
		else if (sv[pos.first - 1 + pos.second * width] == '#') {
			dir = (dir + 1) % 4;
		}
		break;
	}
	s[pos.first + pos.second * width] = 'X';
	visited.insert(pos.first + pos.second * width);
}

void move_vec(std::vector<std::string> in, std::pair<int, int>& pos, int& dir, std::set < std::pair<int, int>>& vis, std::vector<std::string>& s) {
	switch (dir) {
	case UP:
		if (in[pos.second - 1][pos.first] == '.' || in[pos.second - 1][pos.first] == '^')
			pos.second--;
		else if (in[pos.second - 1][pos.first] == '#') {
			dir = (dir + 1) % 4;
		}
		break;
	case RIGHT:
		if (in[pos.second][pos.first + 1] == '.' || in[pos.second][pos.first + 1] == '^')
			pos.first++;
		else if (in[pos.second][pos.first + 1] == '#') {
			dir = (dir + 1) % 4;
		}
		break;
	case DOWN:
		if (in[pos.second + 1][pos.first] == '.' || in[pos.second + 1][pos.first] == '^')
			pos.second++;
		else if (in[pos.second + 1][pos.first] == '#') {
			dir = (dir + 1) % 4;
		}
		break;
	case LEFT:
		if (in[pos.second][pos.first - 1] == '.' || in[pos.second][pos.first - 1] == '^')
			pos.first--;
		else if (in[pos.second][pos.first - 1] == '#') {
			dir = (dir + 1) % 4;
		}
		break;
	}
	s[pos.second][pos.first] = 'X';
	vis.insert(pos);
}

void print_map(const std::vector<std::string>& s) {
	std::cout << '\n';
	for (const auto& st : s) {
		std::cout << st << '\n';
	}
	std::cout << '\n';
}


bool brute_force_vec(std::vector<std::string> in, std::pair<int, int> pos, int dir, std::set<std::pair<int, int>> visited, std::vector<std::string> s, int x, int y) {
	if (in[y][x] == '#' || in[y][x] == '^')
		return false;
	in[y][x] = '#';
	std::set<std::tuple<int, int, int>> hist;
	hist.insert({ pos.first, pos.second, dir });
	while (pos.first != 0 && dir == LEFT || pos.first < in[0].length() - 1 && dir == RIGHT || pos.second != 0 && dir == UP || pos.second < in.size() - 1 && dir == DOWN) {
		move_vec(in, pos, dir, visited, s);
		if (hist.contains({ pos.first, pos.second, dir }))
			return true;
		hist.insert({ pos.first, pos.second, dir });
	}
	return false;
}



int main() {
	std::vector<std::string> in = aoc_utils::read_lines_mmap<std::string>("input.txt");

	std::pair<int, int> pos, pos2;

	for (int i = 0; i < in.size(); i++) {
		if (in[i].find('^') != std::string::npos) {
			pos.first = in[i].find('^');
			pos.second = i;
			break;
		}
	}

	pos2 = pos;

	int dir = UP;

	std::vector<std::string> s{ in };

	std::set<std::pair<int, int>> visited;
	visited.insert(pos);

	while (pos.first != 0 && dir == LEFT || pos.first < in[0].length() - 1 && dir == RIGHT || pos.second != 0 && dir == UP || pos.second < in.size() - 1 && dir == DOWN) {
		move_vec(in, pos, dir, visited, s);
	}

	int p1 = visited.size(), p2 = 0;
	
	s = in;
	dir = UP;

	// GROSS
	p2 = std::transform_reduce(std::execution::par, visited.begin(), visited.end(), 0, std::plus<>(), [&](std::pair<int, int> p) {
		if (brute_force_vec(in, pos2, dir, visited, s, p.first, p.second)) {
			return 1;
		}
		return 0;
		});

	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

}

//Part 1: 5067
//Part 2 : 1793
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 27
//Milliseconds : 96
//Ticks : 270961039
//TotalDays : 0.000313612313657407
//TotalHours : 0.00752669552777778
//TotalMinutes : 0.451601731666667
//TotalSeconds : 27.0961039
//TotalMilliseconds : 27096.1039