#include "../utils/utils.h"


std::pair<uint64_t, uint64_t> split_number(uint64_t number) {
	uint64_t divisor = 1;
	uint64_t length = 0;
	uint64_t temp = number;

	while (temp > 0) {
		temp /= 10;
		length++;
		if (length % 2 == 0) {
			divisor *= 10;
		}
	}

	return { number / divisor, number % divisor };
}


uint64_t count_digits(uint64_t n) {
	int count = 0;
	while (n != 0) {
		n = n / 10;
		++count;
	}
	return count;
}


void solve(uint64_t n, uint64_t& curr, std::map<std::pair<uint64_t, int>, uint64_t>& m, int d, const int lim) {
	if (d > lim) {
		curr += 1;
		return;
	}

	auto key = std::make_pair(n, d);
	if (m.find(key) != m.end()) {
		curr += m[key];
		return;
	}

	uint64_t initial_curr = curr;

	if (n == 0) { solve(1, curr, m, d + 1, lim); }
	else if (count_digits(n) & 1) { solve(n * 2024, curr, m, d + 1, lim); }
	else {
		auto [first, second] = split_number(n);
		solve(first, curr, m, d + 1, lim);
		solve(second, curr, m, d + 1, lim);
	}
	m[key] = curr - initial_curr;
}

int main() {

	INITIALIZE_AOC_TIMERS();

	default_timer.begin(0);

    std::vector<uint64_t> in;

	std::string s = aoc_utils::memory_map_file("input.txt").value();

	for (auto [m, n] : ctre::search_all<R"((\d+))">(s)) {
		in.push_back(n.to_number<uint64_t>());
	}

	default_timer.end(0);

	default_timer.begin(1);
	
	uint64_t p1 = 0, p2 = 0;

	auto in1 = in;

	std::map<std::pair<uint64_t, int>, uint64_t> m1;
	p1 = std::reduce(std::execution::unseq, in.begin(), in.end(), 0ull, [&m1](uint64_t acc, uint64_t i) {
		uint64_t c = 0;
		solve(i, c, m1, 1, 25);
		return acc + c;
		});

	default_timer.end(1);

	default_timer.begin(2);
	std::map<std::pair<uint64_t, int>, uint64_t> m2;
	p2 = std::reduce(std::execution::unseq, in.begin(), in.end(), 0ull, [&m2](uint64_t acc, uint64_t i) {
		uint64_t c = 0;
		solve(i, c, m2, 1, 75);
		return acc + c;
		});

	default_timer.end(2);

	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

	default_timer.display_all();
}

//Part 1: 203609
//Part 2 : 240954878211138
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 130.6 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 633.2 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 48973 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 82
//Ticks : 825465
//TotalDays : 9.55399305555555E-07
//TotalHours : 2.29295833333333E-05
//TotalMinutes : 0.001375775
//TotalSeconds : 0.0825465
//TotalMilliseconds : 82.5465