#include "../utils/utils.h"

// Lookup table for powers of all powers of 10 needed
constexpr std::array<uint64_t, 20> POWERS_OF_10 = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL
};

inline uint32_t count_digits_fast(uint64_t n) {
	for (int i = 0; i < POWERS_OF_10.size(); i++) {
		if (n < POWERS_OF_10[i]) {
			return i;
		}
	}

    return 0; // Never reach
}

inline uint64_t string_to_uint64(std::string_view str) {
    uint64_t result = 0;
    for (char c : str) {
        result = (result << 3) + (result << 1) + (c & 0xF);
    }
    return result;
}

inline uint64_t concat_uint64_fast(uint64_t a, uint64_t b) {
    return a * POWERS_OF_10[count_digits_fast(b)] + b;
}

inline bool rec(const std::vector<uint64_t>& in, const size_t i, const uint64_t& current, const bool p2 = false) {
    if (i == in.size()) {
        return current == in[0];
    }

    const uint64_t next = in[i];

    if (rec(in, i + 1, current + next, p2)) {
        return true;
    }

    if (rec(in, i + 1, current * next, p2)) {
        return true;
    }

    if (p2 && rec(in, i + 1, concat_uint64_fast(current, next), p2)) {
        return true;
    }

    return false;
}

int main() {
    INITIALIZE_AOC_TIMERS();
    default_timer.begin(0);

    std::vector<std::vector<uint64_t>> nums = aoc_utils::read_lines_mmap<std::vector<uint64_t>>("input.txt", [&nums](const std::string& line) {
        std::vector<uint64_t> res;
        for (auto [m, a] : ctre::search_all<R"((\d+))">(line)) {
            res.push_back(string_to_uint64(std::string_view(a.data(), a.size())));
        }
        return res;
        });

    default_timer.end(0);


    default_timer.begin(1);

    const uint64_t p1 = std::transform_reduce(
        std::execution::par_unseq,
        nums.begin(), nums.end(),
        uint64_t(0),
        std::plus<uint64_t>(),
        [](const std::vector<uint64_t>& v) {
            return rec(v, 2, v[1]) ? v[0] : 0;
        }
    );

    default_timer.end(1);
    default_timer.begin(2);

    const uint64_t p2 = std::transform_reduce(
        std::execution::par_unseq,
        nums.begin(), nums.end(),
        uint64_t(0),
        std::plus<uint64_t>(),
        [](const std::vector<uint64_t>& v) {
            return rec(v, 2, v[1], true) ? v[0] : 0;
        }
    );

    default_timer.end(2);

    std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';
    default_timer.display_all();
}

//Part 1: 7885693428401
//Part 2 : 348360680516005
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 689 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 735.4 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 5052.5 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 22
//Ticks : 229579
//TotalDays : 2.65716435185185E-07
//TotalHours : 6.37719444444444E-06
//TotalMinutes : 0.000382631666666667
//TotalSeconds : 0.0229579
//TotalMilliseconds : 22.9579