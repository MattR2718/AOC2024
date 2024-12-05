#include "../utils/utils.h"

int main() {
    
	INITIALIZE_AOC_TIMERS();
	
	uint8_t rules[100][100]{ 0 };
	std::vector<std::vector<int>> updates;

	bool r = true;

	default_timer.begin(0);
	aoc_utils::read_lines_mmap_apply<int>("input.txt", [&](const std::string& line) {
		if (line.empty()) {
			r = false;
			return 0;
		}

		if (r) {
			for (auto [m, a, b] : ctre::search_all<R"((\d+)\|(\d+))">(line)) {
				rules[a.to_number()][b.to_number()] = 1;
			}
		}
		else {
			updates.emplace_back();
			for (auto [m, a] : ctre::search_all<R"((\d+))">(line)) {
				updates.back().emplace_back(a.to_number());
			}
		}

		return 0;
		});

	default_timer.end(0);

	std::atomic<int> p1 = 0, p2 = 0;

	default_timer.begin(1);

	int _ = std::transform_reduce(std::execution::unseq, updates.begin(), updates.end(), 0, std::plus<>(), [&](std::vector<int>& up) {
		for (int i = 0; i < up.size() - 1; ++i) {
			for (int j = i + 1; j < up.size(); j++) {
				if (rules[up[j]][up[i]]) {

					std::sort(std::execution::unseq, up.begin(), up.end(), [&](int a, int b) {
						return rules[a][b];
						});

					p2 += up[up.size() / 2];
					return 0;
				}
			}
		}

		p1 += up[up.size() / 2];
		return 0;
	});

	default_timer.end(1);

	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

	default_timer.display_all();

}

//Part 1: 5208
//Part 2 : 6732
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 451.5 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1+2
//Description : Compute part 1 + 2
//Elapsed Time : 69.2 microseconds
//========================================================================== =
//Timer ID : 2 is still running.
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 16
//Ticks : 166395
//TotalDays : 1.92586805555556E-07
//TotalHours : 4.62208333333333E-06
//TotalMinutes : 0.000277325
//TotalSeconds : 0.0166395
//TotalMilliseconds : 16.6395