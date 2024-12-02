#include "../utils/utils.h"

int main() {
    std::vector<std::vector<int>> in;

	in = aoc_utils::read_lines_mmap<std::vector<int>>("input.txt", [&](const std::string& line) {
		std::vector<std::string> s = aoc_utils::tokenize(line, " ");
		std::vector<int> v;
		for (const std::string& t : s) {
			v.push_back(std::stoi(t));
		}
		return v;
		});

	/*for(auto& v : in) {
		for(int i : v) {
			std::cout << i << ' ';
		}
		std::cout<< '\n';
	}*/

	auto is_valid = [](const auto& v) {
		auto inc = std::adjacent_find(v.begin(), v.end(), [](int a, int b) {
			return a >= b || std::abs(a - b) > 3;
			});

		auto dec = std::adjacent_find(v.begin(), v.end(), [](int a, int b) {
			return a <= b || std::abs(a - b) > 3;
			});

		return (inc != v.end() && dec != v.end() ? 0 : 1);
		};

	int p1 = std::reduce(in.begin(), in.end(), 0, [&is_valid](int acc, const std::vector<int>& v) {
		return acc + is_valid(v);
		});

	std::cout<<"PART 1: "<<p1<<'\n';

	int p2 = std::reduce(in.begin(), in.end(), 0, [&is_valid](int acc, std::vector<int>& v) {
		// Gross brute force
		for (size_t i = 0; i < v.size(); ++i) {
			int current = v[i];
			v.erase(v.begin() + i);

			if (is_valid(v)) {
				return acc + 1;
			}

			v.insert(v.begin() + i, current);
		}

		return acc;
		
		});


	std::cout<<"PART 2: "<<p2<<'\n';


}

//PART 1: 479
//PART 2 : 531
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 19
//Ticks : 195700
//TotalDays : 2.2650462962963E-07
//TotalHours : 5.43611111111111E-06
//TotalMinutes : 0.000326166666666667
//TotalSeconds : 0.01957
//TotalMilliseconds : 19.57