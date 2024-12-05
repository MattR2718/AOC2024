#include "../utils/utils.h"


int check_valid(const std::map<int, std::vector<int>>& rules, const std::vector<int>& update) {
	for(int i = 0; i < update.size() - 1; ++i) {
		for(int j = i + 1; j < update.size(); j++){
			if(rules.contains(update[j]) && std::find(rules.at(update[j]).begin(), rules.at(update[j]).end(), update[i]) != rules.at(update[j]).end()) {
				return 0;
			}
		
		}
	}

	return update[(update.size() + 1) / 2 - 1];
}

int fix(const std::map<int, std::vector<int>>& rules, const std::vector<int>& update) {
	std::vector<int> res = update;

	std::sort(res.begin(), res.end(), [&](int a, int b) {
		return !(rules.contains(b) && std::find(rules.at(b).begin(), rules.at(b).end(), a) != rules.at(b).end());
	});

	return res[(res.size() + 1) / 2 - 1];
}


int main() {
    
	INITIALIZE_AOC_TIMERS();
	
	std::map<int, std::vector<int>> rules;
	std::vector<std::vector<int>> updates;

	bool r = true;


	default_timer.begin(0);
	aoc_utils::read_lines_mmap_apply<int>("input.txt", [&](const std::string& line) {
		if (line.empty()) {
			r = false;
			return 0;
		}

		if (r) {
			for (auto [m, a, b] : ctre::search_all<R"((\d+)\|(\d+))">(line)) {
				rules[a.to_number()].push_back(b.to_number());
			}
		}
		else {
			updates.emplace_back();
			for (auto [m, a] : ctre::search_all<R"((\d+))">(line)) {
				updates.back().emplace_back(a.to_number());
			}
		}

		return 0;
		});

	default_timer.end(0);


	int p1 = 0, p2 = 0;

	std::vector<std::vector<int>> inv;

	default_timer.begin(1);

	p1 = std::reduce(std::execution::seq, updates.begin(), updates.end(), 0, [&](int acc, const std::vector<int>& up) {
		int i = check_valid(rules, up);
		if (!i) {
			inv.emplace_back(up);
		}
		return acc + i;
	});

	default_timer.end(1);

	default_timer.begin(2);

	p2 = std::reduce(std::execution::seq, inv.begin(), inv.end(), 0, [&](int acc, const std::vector<int>& up) {
		return acc + fix(rules, up);
	});

	default_timer.end(2);


	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

	default_timer.display_all();

}

//Part 1: 5208
//Part 2 : 6732
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 848 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 514.5 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 528.7 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 26
//Ticks : 264565
//TotalDays : 3.06209490740741E-07
//TotalHours : 7.34902777777778E-06
//TotalMinutes : 0.000440941666666667
//TotalSeconds : 0.0264565
//TotalMilliseconds : 26.4565