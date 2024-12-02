#include "../utils/utils.h"


template <typename T>
void print_binary(T n) {
	int j = 0;
    for (int i = sizeof(T) * 8 - 1; i >= 0; --i) {
        std::cout << ((n >> i) & 1);
		if (++j % 8 == 0) {
			std::cout << ' ';
		}
    }
    std::cout << '\n';
}

int main() {
	std::vector<uint64_t> in;

	in = aoc_utils::read_lines_mmap<uint64_t>("input.txt", [&](const std::string& line) {
		std::vector<std::string> s = aoc_utils::tokenize(line, " ");
		uint64_t v = 0;
		int i = 64 - 8;
		uint64_t d = 0, d2 = 0;

		if (s.size() > 8) {
			std::cout << "SIZE OF S GREATER THAN 8: \n";
			for (const std::string& t : s) {
				std::cout << t << '\n';
			}
			for(int l = 0; l < 100000; l++) {
				std::cout << "\n\n\n";
			}
		}

		for (const std::string& t : s) {
			d2 = d;
			d = std::stoull(t) << i;
			std::cout << "s: " << t << "  d: " << d << "  bin: ";
			print_binary<uint64_t>(d);
			i -= 8;
			v |= d;
		}

		/*bool inc = d > d2;
		uint64_t n = 1;
		while (i > 0) {
			v |= ((inc) ? d + n : d - 1) << i;
			i -= 8;
		}*/
		std::cout << "\n\n\n";
		return v;
		});

	for(uint64_t& v : in) {
		print_binary<uint64_t>(v);
		std::cout << v << '\n';
	}

	//auto is_valid = [](const auto& v) {
	//	auto inc = std::adjacent_find(v.begin(), v.end(), [](int a, int b) {
	//		return a >= b || std::abs(a - b) > 3;
	//		});

	//	auto dec = std::adjacent_find(v.begin(), v.end(), [](int a, int b) {
	//		return a <= b || std::abs(a - b) > 3;
	//		});

	//	return (inc != v.end() && dec != v.end() ? 0 : 1);
	//	};

	//int p1 = std::reduce(in.begin(), in.end(), 0, [&is_valid](int acc, const std::vector<int>& v) {
	//	return acc + is_valid(v);
	//	});

	//std::cout << "PART 1: " << p1 << '\n';

	//int p2 = std::reduce(in.begin(), in.end(), 0, [&is_valid](int acc, std::vector<int>& v) {
	//	// Gross brute force
	//	for (size_t i = 0; i < v.size(); ++i) {
	//		int current = v[i];
	//		v.erase(v.begin() + i);

	//		if (is_valid(v)) {
	//			return acc + 1;
	//		}

	//		v.insert(v.begin() + i, current);
	//	}

	//	return acc;

	//	});


	//std::cout << "PART 2: " << p2 << '\n';


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