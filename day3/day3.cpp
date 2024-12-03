#include "../utils/utils.h"

int main() {

    aoc_utils::Timer timer;

    aoc_utils::timer_config p12_timer_config = {
            .id = 0,
            .units = "microseconds",
            .label = "Part 12",
            .description = "Memory Map File, then Compute parts 1 and 2"
    };

    timer.create_timer(p12_timer_config);

    timer.begin(0);
    boost::iostreams::mapped_file_source f;
    std::string_view sv;
    aoc_utils::memory_map_file_boost_sv("input.txt", f, sv);
    constexpr auto pattern = ctll::fixed_string{ R"((mul\((\d+),\s*(\d+)\)|do\(\)|don't\(\)))" };

    int pp1 = 0, pp2 = 0;
    bool d = true;

    constexpr std::string_view do_str = "do()";
    constexpr std::string_view dont_str = "don't()";

    int a = 0, b = 0;
	const char* a_start = nullptr, *b_start = nullptr;
    for (const auto& match : ctre::range<pattern>(sv.data())) {
        std::string_view view = match.to_view();
        if (view.starts_with("m")) {

            a = match.get<2>().to_number();
            b = match.get<3>().to_number();

            pp1 += a * b;
            if (d) pp2 += a * b;
        }
        else if (view == do_str) {
            d = true;
        }
        else if (view == dont_str) {
            d = false;
        }
    }
    
    
    timer.end(0);

    std::cout << "Part 1: " << pp1 << '\n' << "Part 2: " << pp2 << '\n';

    timer.display_all();

}

//Part 1: 178538786
//Part 2 : 102467299
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Part 12
//Description : Memory Map File, then Compute parts 1 and 2
//Elapsed Time : 255.5 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 15
//Ticks : 153922
//TotalDays : 1.78150462962963E-07
//TotalHours : 4.27561111111111E-06
//TotalMinutes : 0.000256536666666667
//TotalSeconds : 0.0153922
//TotalMilliseconds : 15.3922