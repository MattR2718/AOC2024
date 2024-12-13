#include "../utils/utils.h"


// Custom hash function for std::pair
struct PairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ (hash2 << 1);
    }
};

using Cache = std::unordered_map<std::pair<uint64_t, int>, uint64_t, PairHash>;

inline uint64_t count_digits(uint64_t number) {
    /*uint64_t count = 0;
    while (number > 0) {
        number /= 10;
        ++count;
    }*/
    return std::log10(number) + 1;
}

inline std::pair<uint64_t, uint64_t> split_number(uint64_t number) {
    uint64_t digits = count_digits(number);
    uint64_t divisor = 1;
    for (uint64_t i = 0; i < digits / 2; ++i) {
        divisor *= 10;
    }
    return { number / divisor, number % divisor };
}

void solve(uint64_t n, uint64_t& curr, Cache& cache, int depth, const int limit) {
    if (depth > limit) {
        curr += 1;
        return;
    }

    auto key = std::make_pair(n, depth);
    if (cache.find(key) != cache.end()) {
        curr += cache[key];
        return;
    }

    uint64_t initial_curr = curr;

    if (n == 0) { solve(1, curr, cache, depth + 1, limit); }
    else if (count_digits(n) % 2 != 0) { solve(n * 2024, curr, cache, depth + 1, limit); }
    else {
        auto [first, second] = split_number(n);
        solve(first, curr, cache, depth + 1, limit);
        solve(second, curr, cache, depth + 1, limit);
    }

    cache[key] = curr - initial_curr;
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

	Cache m1;
	p1 = std::reduce(std::execution::unseq, in.begin(), in.end(), 0ull, [&m1](uint64_t acc, uint64_t i) {
		uint64_t c = 0;
		solve(i, c, m1, 1, 25);
		return acc + c;
		});

	default_timer.end(1);

	default_timer.begin(2);
	Cache m2;
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
//Elapsed Time : 122.4 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 275.5 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 13737.5 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 56
//Ticks : 569139
//TotalDays : 6.58725694444444E-07
//TotalHours : 1.58094166666667E-05
//TotalMinutes : 0.000948565
//TotalSeconds : 0.0569139
//TotalMilliseconds : 56.9139