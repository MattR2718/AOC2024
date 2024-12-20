#include "../utils/utils.h"

#include <numeric>
#include <set>

struct pt {
	int x, y;

	bool operator<(const pt& other) const {
		return std::tie(x, y) < std::tie(other.x, other.y);
	}

	bool operator==(const pt& other) const {
		return std::tie(x, y) == std::tie(other.x, other.y);
	}

	friend std::ostream& operator << (std::ostream& os, pt p) {
		os << p.x << ',' << p.y;
		return os;
	}

	std::string string() const {
		return std::to_string(x) + ',' + std::to_string(y);
	}

	struct hash {
		size_t operator()(const pt& p) const {
			return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
		}
	};
};

std::unordered_map<pt, pt, pt::hash> bfs(const std::unordered_map<pt, std::vector<pt>, pt::hash>& adj, const pt& start, const pt& end) {
	std::queue<pt> q;
	std::unordered_set<pt, pt::hash> visited;
	std::unordered_map<pt, pt, pt::hash> from;
	q.push(start);
	visited.insert(start);
	while (!q.empty()) {
		pt current = q.front();
		q.pop();
		if (current == end) {
			std::vector<pt> path;
			while (current != start) {
				path.push_back(current);
				current = from[current];
			}
			path.push_back(start);
			std::reverse(path.begin(), path.end());
			return from;
		}
		for (const auto& next : adj.at(current)) {
			if (!visited.count(next)) {
				q.push(next);
				visited.insert(next);
				from[next] = current;
			}
		}
	}
	return from;
}

std::unordered_map<pt, std::vector<pt>, pt::hash> make_adj(const std::vector<std::string>& lines) {
	std::unordered_map<pt, std::vector<pt>, pt::hash> adj;
	for (int j = 0; j < lines.size(); j++) {
		for (int i = 0; i < lines[j].size(); i++) {
			if (lines[j][i] == '#') continue;
			pt p = { i, j };
			if (lines[j][i] == '.') {
				if (j > 0 && lines[j - 1][i] == '.') adj[p].push_back({ i, j - 1 });
				if (i > 0 && lines[j][i - 1] == '.') adj[p].push_back({ i - 1, j });
				if (j < lines.size() - 1 && lines[j + 1][i] == '.') adj[p].push_back({ i, j + 1 });
				if (i < lines[j].size() - 1 && lines[j][i + 1] == '.') adj[p].push_back({ i + 1, j });
			}
		}
	}
	return adj;
}

int main() {

	aoc_utils::Timer timer;
	aoc_utils::timer_config input_timer_config = {
		.id = 0,
		.units = "microseconds",
		.label = "Input",
		.description = "Read input from file and parse"
	};
	aoc_utils::timer_config bfs_timer_config = {
		.id = 2,
		.units = "microseconds",
		.label = "BFS",
		.description = "Find Start and End + BFS"
	};
	aoc_utils::timer_config adj_timer_config = {
		.id = 1,
		.units = "microseconds",
		.label = "ADJ",
		.description = "Make adj"
	};
	aoc_utils::timer_config p12_timer_config = {
		.id = 3,
		.units = "microseconds",
		.label = "Part 1 + 2",
		.description = "Compute parts 1 and 2"
	};
	timer.create_timer(input_timer_config);
	timer.create_timer(bfs_timer_config);
	timer.create_timer(adj_timer_config);
	timer.create_timer(p12_timer_config);



	timer.begin(0);


	std::vector<std::string> lines = aoc_utils::read_lines("input.txt");

	timer.end(0);

	pt start, end;
	for (int j = 0; j < lines.size(); j++) {
		for (int i = 0; i < lines[j].size(); i++) {
			if (lines[j][i] == 'S') {
				start = { i, j };
			}
			if (lines[j][i] == 'E') {
				end = { i, j };
			}
		}
	}

	lines[start.y][start.x] = '.';
	lines[end.y][end.x] = '.';
	
	std::atomic<int> p1 = 0, p2 = 0;

	timer.begin(1);

	auto adj = make_adj(lines);

	timer.end(1);

	timer.begin(2);
	auto from = bfs(adj, start, end);

	auto current = end;
	std::vector<pt> path;
	while (current != start) {
		path.push_back(current);
		current = from[current];
	}
	path.push_back(start);

	timer.end(2);

	timer.begin(3);

	for (int i = 0; i < path.size() - 1; i++) {
		for (int j = i; j < path.size(); j++) {
			int man_dist = abs(path[i].x - path[j].x) + abs(path[i].y - path[j].y);
			if (man_dist < 3 && (j - i - man_dist) >= 100) p1++;
			if (man_dist < 21 && (j - i - man_dist) >= 100) p2++;
		}
	}
	
	timer.end(3);

	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

	timer.display_all();

}

//Part 1: 1459
//Part 2 : 1016066
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 289 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : BFS
//Description : Find Start and End + BFS
//Elapsed Time : 2884.9 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : ADJ
//Description : Make adj
//Elapsed Time : 1831.1 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 3
//Label : Part 1 + 2
//Description : Compute parts 1 and 2
//Elapsed Time : 35883.6 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 60
//Ticks : 608976
//TotalDays : 7.04833333333333E-07
//TotalHours : 1.6916E-05
//TotalMinutes : 0.00101496
//TotalSeconds : 0.0608976
//TotalMilliseconds : 60.8976