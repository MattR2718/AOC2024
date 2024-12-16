#include "../utils/utils.h"

#include <set>

enum DIR {
	UP, RIGHT, DOWN, LEFT
};

struct Entry {
	int x;
	int y;
	DIR dir;

	bool operator<(const Entry& other) const {
		if (x != other.x) return x < other.x;
		if (y != other.y) return y < other.y;
		return dir < other.dir;
	}

	bool operator==(const Entry& other) const {
		return x == other.x && y == other.y && dir == other.dir;
	}
};

std::tuple<std::pair<int, int>, std::pair<int, int>> find_start_end(const std::vector<std::string>& input) {
	std::pair<int, int> start;
	std::pair<int, int> end;

	for(int i = 0; i < input.size(); i++) {
		for(int j = 0; j < input[i].size(); j++) {
			if(input[i][j] == 'S') {
				start = {j, i};
			}
			if(input[i][j] == 'E') {
				end = {j, i};
			}
		}
	}

	return {start, end};
}

std::map<Entry, std::vector<std::pair<Entry, int>>> get_adjacency_list(const std::vector<std::string>& input, int x, int y, DIR dir) {

    std::map<Entry, std::vector<std::pair<Entry, int>>> adj_list;
    std::set<Entry> visited;

    std::stack<Entry> stack;
    stack.push({ x, y, dir });

    while (!stack.empty()) {
        Entry e = stack.top();
        stack.pop();

        if (visited.find(e) != visited.end()) {
            continue;
        }
        visited.insert(e);

        if (adj_list.find(e) == adj_list.end()) {
            adj_list[e] = {};
        }

        static const std::vector<std::tuple<int, int, DIR>> directions = {
            {0, -1, UP},
            {1, 0, RIGHT},
            {0, 1, DOWN},
            {-1, 0, LEFT}
        };

        for (const auto& [dx, dy, new_dir] : directions) {
            int nx = e.x + dx;
            int ny = e.y + dy;

            if (nx < 0 || ny < 0 || nx >= input[0].size() || ny >= input.size()) {
                continue;
            }

            if (input[ny][nx] == '#') {
                continue;
            }

            Entry neighbor = { nx, ny, new_dir };

            if (visited.find(neighbor) == visited.end()) {
                stack.push(neighbor);
            }

            int cost = (e.dir == new_dir) ? 1 : 1001;
            if ((e.dir == UP && new_dir == DOWN) || (e.dir == DOWN && new_dir == UP) ||
                (e.dir == LEFT && new_dir == RIGHT) || (e.dir == RIGHT && new_dir == LEFT)) {
                cost = 2001;
            }

            adj_list[e].push_back({ neighbor, cost });
        }
    }

    return adj_list;
}

int dijkstra(const std::map < Entry, std::vector<std::pair<Entry, int>>>& adj, const Entry& start, const std::pair<int, int>& end, std::map<Entry, int> dists, std::map<Entry, std::vector<Entry>>& paths) {

	for(const auto& [k, v] : adj) {
		dists[k] = INT_MAX;
		paths[k] = {};
	}

	dists[start] = 0;

	std::priority_queue<std::pair<int, Entry>, std::vector<std::pair<int, Entry>>, std::greater<std::pair<int, Entry>>> pq;

	pq.push({0, start});

	while (!pq.empty()) {
		int curr_dist = pq.top().first;
		Entry curr_node = pq.top().second;
		if(curr_node.x == end.first && curr_node.y == end.second) {
			break;
		}
		pq.pop();

		if (curr_dist > dists[curr_node]) {
			continue;
		}

		for (const auto& [neighbor, weight] : adj.at(curr_node)) {
			int new_dist = curr_dist + weight;

			if (new_dist < dists[neighbor]) {
				dists[neighbor] = new_dist;
				pq.push({ new_dist, neighbor });

				// New shortest path to get here
				paths[neighbor].clear();
				paths[neighbor].push_back(curr_node);
			}
			else if (new_dist == dists[neighbor]) {
				paths[neighbor].push_back(curr_node);
			}
		}
	}
	return pq.top().first;
}

void find_paths(const Entry& source, Entry vertex,const std::map<Entry, std::vector<Entry>>& paths, std::set<std::pair<int, int>>& unique) {
	unique.insert({ vertex.x, vertex.y });
	if (vertex != source && paths.contains(vertex)) {
		for (Entry predecessor : paths.at(vertex)) {
			find_paths(source, predecessor, paths, unique);
		}
	}
}

int main() {

	INITIALIZE_AOC_TIMERS();


	default_timer.begin(0);

	std::vector<std::string> input = aoc_utils::read_lines_mmap<std::string>("input.txt");

	auto [start, end] = find_start_end(input);

	auto adj_list = get_adjacency_list(input, start.first, start.second, RIGHT);

	default_timer.end(0);

	default_timer.begin(1);

	std::map<Entry, std::vector<Entry>> paths;
	int p1 = dijkstra(adj_list, {start.first, start.second, RIGHT}, end, {}, paths);

	default_timer.end(1);

	default_timer.begin(2);

	std::set<std::pair<int, int>> unique;
	find_paths({ start.first, start.second, RIGHT }, { end.first, end.second, UP }, paths, unique);
	find_paths({ start.first, start.second, RIGHT }, { end.first, end.second, RIGHT }, paths, unique);

	/*
	
	Assuming ending is always top right, only able to approach from up or right.
	My input only goes right but keeping it somewhat general

	find_paths({ start.first, start.second, RIGHT }, { end.first, end.second, DOWN }, paths, unique);
	find_paths({ start.first, start.second, RIGHT }, { end.first, end.second, LEFT }, paths, unique);
	*/

	int p2 = unique.size();

	default_timer.end(2);


	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';


	default_timer.display_all();


}

//Part 1: 160624
//Part 2 : 692
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 34704.8 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 32825.6 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 12050.2 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 113
//Ticks : 1133503
//TotalDays : 1.31192476851852E-06
//TotalHours : 3.14861944444444E-05
//TotalMinutes : 0.00188917166666667
//TotalSeconds : 0.1133503
//TotalMilliseconds : 113.3503