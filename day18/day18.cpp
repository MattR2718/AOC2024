#include "../utils/utils.h"

#include <set>

#define test false

struct pt {
	int x, y;

	bool operator<(const pt& other) const {
		return std::tie(x, y) < std::tie(other.x, other.y);
	}

	bool operator==(const pt& other) const {
		return std::tie(x, y) == std::tie(other.x, other.y);
	}

	std::string string() const {
		return std::to_string(x) + ',' + std::to_string(y);
	}
};

std::map<pt, std::vector<pt>> get_adj(std::vector<std::string>& lines) {
	std::map<pt, std::vector<pt>> adj;
	for (int i = 0; i < lines.size(); i++) {
		for (int j = 0; j < lines[i].size(); j++) {
			if (lines[i][j] == '#') continue;
			pt p = { j, i };
			std::vector<pt> v;
			if (i > 0 && lines[i - 1][j] != '#') v.push_back({ j, i - 1 });
			if (i < lines.size() - 1 && lines[i + 1][j] != '#') v.push_back({ j, i + 1 });
			if (j > 0 && lines[i][j - 1] != '#') v.push_back({ j - 1, i });
			if (j < lines[i].size() - 1 && lines[i][j + 1] != '#') v.push_back({ j + 1, i });
			adj[p] = v;
		}
	}
	return adj;
}

std::vector<std::string> make_map(const std::vector<pt>& ls, int num_bytes) {
	int max_x = 0, max_y = 0;
	for (auto l : ls) {
		max_x = std::max(max_x, l.x);
		max_y = std::max(max_y, l.y);
	}
	if (max_x > 10 || max_y > 10) {
		max_x = 70;
		max_y = 70;
	}
	else {
		max_x = 6;
		max_y = 6;
	}
	std::vector<std::string> map(max_y + 1, std::string(max_x + 1, '.'));
	int i = 0;
	for (auto l : ls) {
		if (i++ == num_bytes) break;
		map[l.y][l.x] = '#';
	}
	return map;
}

int bfs(std::map<pt, std::vector<pt>>& adj, pt start, pt end) {
	std::queue<std::pair<pt, int>> q;
	std::set<pt> visited;

	q.push({ start, 0 });
	visited.insert(start);

	while (!q.empty()) {
		auto [p, dist] = q.front();
		q.pop();

		if (p == end) return dist;

		for (auto n : adj[p]) {
			if (visited.count(n)) continue;
			visited.insert(n);
			q.push({ n, dist + 1 });
		}
	}
	return -1;
}

int main() {

    INITIALIZE_AOC_TIMERS();

    default_timer.begin(0);

#if test
	auto ls = aoc_utils::read_lines_mmap<pt>("test.txt", [](const std::string& s) {
#else
	auto ls = aoc_utils::read_lines_mmap<pt>("input.txt", [](const std::string& s) {
#endif
		
		auto [m, x, y] = ctre::match<R"((\d+),(\d+))">(s);
		return pt{ x.to_number(), y.to_number() };
		});

#if test
	auto lines = make_map(ls, 12);
#else
	auto lines = make_map(ls, 1024);
#endif

	auto adj = get_adj(lines);

    default_timer.end(0);

	int p1 = 0;
	std::string p2 = "";
	int p2i = 0;


	default_timer.begin(1);

#if test
	p1 = bfs(adj, pt{0, 0}, pt{6, 6});
#else
	p1 = bfs(adj, pt{ 0, 0 }, pt{ 70, 70 });
#endif

	default_timer.end(1);

	default_timer.begin(2);

	for (int i = ls.size(); i > 0; i--) {
		auto lines2 = make_map(ls, i);
		auto adj2 = get_adj(lines2);
#if test
		p2i = bfs(adj2, pt{ 0, 0 }, pt{ 6, 6 });
#else
		p2i = bfs(adj2, pt{ 0, 0 }, pt{ 70, 70 });
#endif
		if (p2i != -1) {
			p2 = ls[i].string();;
			break;
		}
	}

	default_timer.end(2);


    std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

    default_timer.display_all();

}

//Part 1: 264
//Part 2 : 41, 26
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 1634.8 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 1528.3 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 179917.2 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 201
//Ticks : 2016407
//TotalDays : 2.33380439814815E-06
//TotalHours : 5.60113055555556E-05
//TotalMinutes : 0.00336067833333333
//TotalSeconds : 0.2016407
//TotalMilliseconds : 201.6407