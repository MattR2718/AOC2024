#include "../utils/utils.h"

uint64_t is_valid(std::string_view towel, const std::vector<std::string>& patterns, std::unordered_map<std::string_view, uint64_t>& cache) {
    if (towel.empty()) return 1;

    auto it = cache.find(towel);
    if (it != cache.end()) {
        return it->second;
    }

    uint64_t n = 0;
    for (const auto& p : patterns) {
        if (towel.length() < p.length()) continue;

        if (towel.compare(0, p.length(), p) != 0) continue;

        if (towel.length() == p.length()) {
            n++;
        }
        else {
            n += is_valid(towel.substr(p.length()), patterns, cache);
        }
    }

    return cache[towel] = n;
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

    std::atomic<uint64_t> p1 = 0, p2 = 0;

	uint64_t _ = std::transform_reduce(std::execution::par_unseq, patterns.begin(), patterns.end(), 0, std::plus<uint64_t>(), [&](const std::string& p) {
        std::unordered_map<std::string_view, uint64_t> cache;

        uint64_t n = is_valid(p, towels, cache);
        p1 += n > 0;
        p2 += n;
        return 0;
		});

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
//Elapsed Time : 387.8 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1 + 2
//Description : Compute parts 1 and 2
//Elapsed Time : 5994.5 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 21
//Ticks : 218630
//TotalDays : 2.53043981481481E-07
//TotalHours : 6.07305555555556E-06
//TotalMinutes : 0.000364383333333333
//TotalSeconds : 0.021863
//TotalMilliseconds : 21.863