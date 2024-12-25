#include "../utils/utils.h"

#include <algorithm>

std::vector<uint8_t> count_hashes(const std::vector<std::string>& grid) {
	if (grid.empty()) return {};

	size_t cols = grid[0].size();
	std::vector<uint8_t> result(cols, 0);

	for (const auto& row : grid) {
		for (size_t col = 0; col < cols; ++col) {
			if (row[col] == '#') {
				result[col]++;
			}
		}
	}

	return result;
}

int main() {

	aoc_utils::Timer timer; \
	aoc_utils::timer_config input_timer_config = { \
		.id = 0, \
		.units = "microseconds", \
		.label = "Input", \
		.description = "Read input from file and parse" \
	}; \
	aoc_utils::timer_config p1_timer_config = { \
		.id = 1, \
		.units = "microseconds", \
		.label = "Part 1", \
		.description = "Compute part 1" \
	}; \

	timer.create_timer(input_timer_config); \
	timer.create_timer(p1_timer_config);
	
	timer.begin(0);

    std::vector<std::vector<std::string>> in;

	std::vector<std::string> acc;
	aoc_utils::read_lines_mmap_apply<void>("input.txt", [&](const std::string& line) {
		if (line.empty()) {
			in.push_back(acc);
			acc.clear();
		}
		else {
			acc.push_back(line);
		}
		});

	in.emplace_back(acc);

	std::vector<std::vector<uint8_t>> locks;
	std::vector<std::vector<uint8_t>> keys;

	for (auto& v : in) {
		if (std::all_of(v[0].begin(), v[0].end(), [](char c) { return c == '#'; })) {
			locks.emplace_back(count_hashes(v));

		}
		else {
			keys.emplace_back(count_hashes(v));
		}
	}

	int height = in[0].size();

	timer.end(0);

	timer.begin(1);

	int p1 = 0;
	for (size_t i = 0; i < locks.size(); ++i) {
		for (size_t j = 0; j < keys.size(); ++j) {
			bool valid = true;
			for (int k = 0; k < locks[i].size(); ++k) {
				if (locks[i][k] + keys[j][k] > height) {
					valid = false;
					break;
				}
			}
			if (valid) {
				p1++;
			}
		}
	}

	timer.end(1);

	std::cout << "Part 1: " << p1 << '\n';

	timer.display_all();

}

//Part 1: 3155
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 363.4 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 325.6 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 15
//Ticks : 156539
//TotalDays : 1.81179398148148E-07
//TotalHours : 4.34830555555556E-06
//TotalMinutes : 0.000260898333333333
//TotalSeconds : 0.0156539
//TotalMilliseconds : 15.6539