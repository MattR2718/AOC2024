#include "../utils/utils.h"

uint64_t is_valid(std::string towel, const std::vector<std::string>& patterns, std::map<std::string, uint64_t>& cache) {
	if (cache.find(towel) != cache.end()) {
		return cache[towel];
	}
	uint64_t n = 0;
	bool t = false;
	for (auto& p : patterns) {
		if (towel == p) {
			n++;
			t = true;
		}
		else if (towel.length() >= p.length() && towel.substr(0, p.length()) == p) {
			uint64_t ret = is_valid(towel.substr(p.length()), patterns, cache);
			if (ret) {
				t = true;
				n += ret;
				cache[towel.substr(p.length())] = ret;
			}
		}
	}

	if (t) { cache[towel] += n; }
	else { cache[towel] = 0; }
	return cache[towel];
}

int main() {

	aoc_utils::Timer timer;

	aoc_utils::timer_config input_timer_config = {
			.id = 0,
			.units = "microseconds",
			.label = "Input",
			.description = "Read in and parse input"
	};

	aoc_utils::timer_config p12_timer_config = {
			.id = 1,
			.units = "microseconds",
			.label = "Part 1 + 2",
			.description = "Compute parts 1 and 2"
	};

	timer.create_timer(input_timer_config);
	timer.create_timer(p12_timer_config);

	timer.begin(0);

	std::vector<std::string> towels;
	std::vector<std::string> patterns;
	bool is_pattern = false;

	aoc_utils::read_lines_mmap_apply<uint64_t>("input.txt", [&](const std::string& line) {
		if (line.empty()) {
			is_pattern = true;
			return 0;
		}

		for (auto [m, s] : ctre::search_all<R"((\w+))">(line)) {
			if (is_pattern) {
				patterns.push_back(s.to_string());
			}
			else {
				towels.push_back(s.to_string());
			}
		}

		return 0;
		});	

	timer.end(0);

	timer.begin(1);

	uint64_t p1 = 0, p2 = 0;

	std::map<std::string, uint64_t> cache;

	for (auto& p : patterns) {
		uint64_t n = is_valid(p, towels, cache);
		p1 += n ? 1 : 0;
		p2 += n;
	}

	timer.end(1);


	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

	timer.display_all();
}

//Part 1: 285
//Part 2 : 636483903099279
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read in and parse input
//Elapsed Time : 443 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1 + 2
//Description : Compute parts 1 and 2
//Elapsed Time : 88141.9 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 108
//Ticks : 1087521
//TotalDays : 1.25870486111111E-06
//TotalHours : 3.02089166666667E-05
//TotalMinutes : 0.001812535
//TotalSeconds : 0.1087521
//TotalMilliseconds : 108.7521