#include "../utils/utils.h"

#include <set>
#include <iterator>

uint64_t evolve(uint64_t n) {
	n ^= n << 6;
	n &= 0xFFFFFF;
	n ^= n >> 5;
	n &= 0xFFFFFF;
	n ^= n << 11;
	n &= 0xFFFFFF;
	return n;
}

uint64_t evolve_2000(uint64_t n) {
	for (uint64_t i = 0; i < 2000; i++) {
		n = evolve(n);
	}
	return n;
}

int main() {

	INITIALIZE_AOC_TIMERS();

	default_timer.begin(0);

	std::vector<uint64_t> in = aoc_utils::read_lines_mmap<uint64_t>("input.txt", [&](const std::string& line) {
		return std::stoull(line);
		});

	default_timer.end(0);

	default_timer.begin(1);


	uint64_t p1 = std::transform_reduce(std::execution::par, in.begin(), in.end(), 0ULL, std::plus<uint64_t>(), evolve_2000);

	default_timer.end(1);

	default_timer.begin(2);

	std::vector<std::vector<int>> diffs;
	for (const auto& sn : in) {
		std::vector<int> diff;
		uint64_t n = sn;
		uint64_t prev = sn;
		for (uint64_t i = 0; i < 2000; i++) {
			n = evolve(n);
			diff.push_back(n % 10 - prev % 10);
			prev = n;
		}
		diffs.push_back(diff);
	}

	std::set<std::array<int, 4>> options;
	for (const auto& diff_v : diffs) {
		for (int i = 0; i < diff_v.size() - 4; i++) {
			std::array<int, 4> option = { diff_v[i], diff_v[i + 1], diff_v[i + 2], diff_v[i + 3] };
			options.insert(option);
		}
	}

	std::map<uint64_t, std::vector<uint64_t>> cache;
	for (const auto& sn : in) {
		uint64_t n = sn;
		std::vector<uint64_t> numbers = { n };
		for (int i = 0; i < 2000; i++) {
			n = evolve(n);
			numbers.push_back(n);
		}
		cache[sn] = numbers;
	}


	std::vector<int> num_bananas;
	int i = 0;
	for (const auto& option : options) {
		//std::cout << ++i << "/" << options.size() << '\n';
		int temp = 0;

		auto process_number = [&option, &cache](uint64_t start) {
			uint64_t n = start;
			std::vector<uint64_t> numbers = cache[start];

			for (size_t i = 0; i < numbers.size() - 4; i++) {
				bool matches = true;
				for (int j = 0; j < 4; j++) {
					int diff = (numbers[i + j + 1] % 10) - (numbers[i + j] % 10);
					if (diff != option[j]) {
						matches = false;
						break;
					}
				}
				if (matches) {
					return numbers[i + 4] % 10;
				}
			}
			return 0ull;
		};

		auto v = std::transform_reduce(
			std::execution::par,
			in.begin(),
			in.end(),
			0ULL,
			std::plus<>(),
			process_number
		);

		num_bananas.push_back(v);

	}


	default_timer.end(2);

	int p2 = *std::max_element(num_bananas.begin(), num_bananas.end());

	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

	default_timer.display_all();

}

// LAPTOP
//Part 1: 15335183969
//Part 2 : 1696
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 255.5 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 2323.5 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 131193210.3 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 2
//Seconds : 11
//Milliseconds : 236
//Ticks : 1312364546
//TotalDays : 0.00151894044675926
//TotalHours : 0.0364545707222222
//TotalMinutes : 2.18727424333333
//TotalSeconds : 131.2364546
//TotalMilliseconds : 131236.4546

// DESKTOP
//Part 1: 15335183969
//Part 2 : 1696
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 630 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 2725.3 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 43547679.5 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 43
//Milliseconds : 589
//Ticks : 435892185
//TotalDays : 0.00050450484375
//TotalHours : 0.01210811625
//TotalMinutes : 0.726486975
//TotalSeconds : 43.5892185
//TotalMilliseconds : 43589.2185