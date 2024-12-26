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

int encode(int a, int b, int c, int d) {
	return ((a + 9) << 15) | ((b + 9) << 10) | ((c + 9) << 5) | (d + 9);
}

std::vector<int> decode(int n) {
	std::vector<int> res;
	res.push_back((n >> 15) - 9);
	res.push_back(((n >> 10) & 0x1F) - 9);
	res.push_back(((n >> 5) & 0x1F) - 9);
	res.push_back((n & 0x1F) - 9);
	return res;
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

	std::vector<int> num_bananas;
	std::vector<int> prices;
	std::vector<int> diffs;
	std::vector<int> seq_outs(32 * 32 * 32 * 32 + 1, 0);
	std::vector<int> seq_outs_input(32 * 32 * 32 * 32 + 1, 0);
	for (const auto& sn : in) {
		prices.clear();
		diffs.clear();
		prices.push_back(sn % 10);
		uint64_t n = sn;
		uint64_t prev = sn;
		for (uint64_t i = 0; i < 2000; i++) {
			n = evolve(n);
			int diff = n % 10 - prev % 10;
			diffs.push_back(diff);
			prices.push_back(n % 10);
			prev = n;
		}

		for (auto& v : seq_outs_input)
			v = 0;

		for (int i = 0; i < diffs.size() - 4; i++) {
			int idx = encode(diffs[i], diffs[i + 1], diffs[i + 2], diffs[i + 3]);
			if(!seq_outs_input[idx])
				seq_outs_input[idx] = prices[i + 4];
		}
		std::transform(std::execution::unseq, seq_outs_input.begin(), seq_outs_input.end(), seq_outs.begin(), seq_outs.begin(), std::plus<int>());
	}


	default_timer.end(2);

	int p2 = *std::max_element(seq_outs.begin(), seq_outs.end());


	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

	default_timer.display_all();

}

//Part 1: 15335183969
//Part 2 : 1696
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 173.9 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 2163.5 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 862951.5 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 884
//Ticks : 8846078
//TotalDays : 1.02385162037037E-05
//TotalHours : 0.000245724388888889
//TotalMinutes : 0.0147434633333333
//TotalSeconds : 0.8846078
//TotalMilliseconds : 884.6078