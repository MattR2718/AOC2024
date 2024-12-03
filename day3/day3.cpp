#include "../utils/utils.h"

int main() {

    aoc_utils::Timer default_timer;
        aoc_utils::timer_config input_timer_config = {
            .id = 0,
            .units = "microseconds",
            .label = "Input",
            .description = "Read input from file and parse into uint64_t"
    };
        aoc_utils::timer_config p1_timer_config = {
            .id = 1,
            .units = "nanoseconds",
            .label = "Part 1",
            .description = "Compute part 1"
    }; 
        aoc_utils::timer_config p2_timer_config = {
            .id = 2,
            .units = "nanoseconds",
            .label = "Part 2",
            .description = "Compute part 2"
        };

        default_timer.create_timer(input_timer_config);
        default_timer.create_timer(p1_timer_config);
        default_timer.create_timer(p2_timer_config);

	std::vector<std::pair<int, int>> in1, in2;

 	bool d = true;

    default_timer.begin(0);
	aoc_utils::read_lines_mmap_apply<int>("input.txt", [&](const std::string& line) {
        
        constexpr auto pattern = ctll::fixed_string{ R"((mul\((\d+),\s*(\d+)\)|do\(\)|don't\(\)))" };

        for (auto match : ctre::range<pattern>(line)) {
            if (match.to_view().starts_with("mul")) {
                int a = std::stoi(std::string{ match.get<2>() });
                int b = std::stoi(std::string{ match.get<3>() });
                in1.emplace_back(a, b);
                if (d) {
                    in2.emplace_back(a, b);
                }
            }
            else if (match.to_view() == "do()") {
                d = true;
            }
            else if (match.to_view() == "don't()") {
                d = false;
            }
        }
		return 0;
		});

    default_timer.end(0);
	
    default_timer.begin(1);
	int p1 = std::reduce(std::execution::seq, in1.begin(), in1.end(), 0, [](int acc, const std::pair<int, int>& p) {
		return acc + p.first * p.second;
		});
    default_timer.end(1);

    default_timer.begin(2);
    int p2 = std::reduce(std::execution::seq, in2.begin(), in2.end(), 0, [](int acc, const std::pair<int, int>& p) {
        return acc + p.first * p.second;
        });

    default_timer.end(2);

    std::cout << "Part 1: " << p1 << "\n";
    std::cout<< "Part 2: " << p2 << "\n";


    default_timer.display_all();

}

//Part 1: 178538786
//Part 2 : 102467299
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse into uint64_t
//Elapsed Time : 808.6 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 700 nanoseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 400 nanoseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 28
//Ticks : 284854
//TotalDays : 3.2969212962963E-07
//TotalHours : 7.91261111111111E-06
//TotalMinutes : 0.000474756666666667
//TotalSeconds : 0.0284854
//TotalMilliseconds : 28.4854