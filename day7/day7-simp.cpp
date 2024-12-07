#include "../utils/utils.h"

inline uint64_t concat_uint64(uint64_t a, uint64_t b) {
	for (uint64_t temp = b; temp != 0; temp /= 10) {
        a *= 10;
	}
	return a + b;
}

inline bool rec(const std::vector<uint64_t>& in, const size_t i, const uint64_t& current, const bool p2 = false) {
    if (i == in.size()) { return current == in[0]; }
	if (current > in[0]) { return false; }

    const uint64_t next = in[i];

    if (rec(in, i + 1, current + next, p2)) { return true; }

    if (rec(in, i + 1, current * next, p2)) { return true; }

    if (p2 && rec(in, i + 1, concat_uint64(current, next), p2)) { return true; }

    return false;
}

int main() {
    INITIALIZE_AOC_TIMERS();
    default_timer.begin(0);

    std::vector<std::vector<uint64_t>> nums = aoc_utils::read_lines_mmap_par<std::vector<uint64_t>>("input.txt", [&nums](const std::string& line) {
        std::vector<uint64_t> res;
        for (auto [m, a] : ctre::search_all<R"((\d+))">(line)) {
            res.push_back(a.to_number<uint64_t>());
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
//Elapsed Time : 821.4 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 315.8 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 3705.4 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 26
//Ticks : 263964
//TotalDays : 3.05513888888889E-07
//TotalHours : 7.33233333333333E-06
//TotalMinutes : 0.00043994
//TotalSeconds : 0.0263964
//TotalMilliseconds : 26.3964