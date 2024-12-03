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
    std::string f = aoc_utils::memory_map_file_boost("input.txt").value();
    constexpr auto pattern = ctll::fixed_string{ R"((mul\((\d+),\s*(\d+)\)|do\(\)|don't\(\)))" };

    int pp1 = 0, pp2 = 0;
    bool d = true;
    for (const auto& match : ctre::range<pattern>(f)) {
        if (match.to_view().starts_with("mul")) {
            int a = std::stoi(std::string{ match.get<2>() });
            int b = std::stoi(std::string{ match.get<3>() });
            pp1 += a * b;
            if (d) {
                pp2 += a * b;
            }
        }
        else if (match.to_view() == "do()") {
            d = true;
        }
        else if (match.to_view() == "don't()") {
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
//Elapsed Time : 359.7 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 19
//Ticks : 195486
//TotalDays : 2.26256944444444E-07
//TotalHours : 5.43016666666667E-06
//TotalMinutes : 0.00032581
//TotalSeconds : 0.0195486
//TotalMilliseconds : 19.5486