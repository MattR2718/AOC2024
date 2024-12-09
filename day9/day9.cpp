#include "../utils/utils.h"

#include <set>

struct entry {
	bool file;
	int id = -1;
	int width;
};

void print(const std::vector<int>& v) {
	for (const int i : v) {
		std::cout << (i == 0 ? "." : std::to_string(i - 1));
	}
	std::cout << '\n';
}

int main() {
	
	INITIALIZE_AOC_TIMERS();


	default_timer.begin(0);

	std::string_view sv;
	boost::iostreams::mapped_file_source file;
	aoc_utils::memory_map_file_boost_sv("input.txt", file, sv);

	std::vector<int> in;
	bool f = true;
	int id = 1;
	for (const char c : sv) {
		if (f) {
			for (int i = 0; i < c - '0'; i++) {
				in.push_back(id);
			}
			id++;
		}
		else {
			for (int i = 0; i < c - '0'; i++) {
				in.push_back(0);
			}
		}
		f = !f;
	}

	default_timer.end(0);
	

	default_timer.begin(1);
	uint64_t p1 = 0;

	size_t p = 0;
	const size_t n = in.size();

	for (size_t i = n - 1; i > 0; i--) {
		if (in[i] != 0) {
			while (p < i && in[p] != 0) {
				p++;
			}
			if (p >= i) break;

			in[p] = in[i];
			in[i] = 0;
		}
	}

	for (int i = 0; i < in.size(); i++) {
		if (in[i] == 0) { break; }
		p1 += i * (in[i] - 1);
	}

	default_timer.end(1);

	default_timer.begin(2);

	uint64_t p2 = 0;

	std::vector<entry> fs;
	f = false;
	id = 0;
	for (const char c : sv) {
		fs.emplace_back(entry{ (f = !f, f), (f ? id++ : -1), c - '0'});
	}

	for (int i = fs.size() - 1; i > 0; i--) {
		if (fs[i].file) {
			for (int j = 0; j < i; j++) {
				if (!fs[j].file && fs[j].width >= fs[i].width) {
					fs[j].width -= fs[i].width;
					entry e = fs[i];
					fs[i].file = false;
					fs[i].id = -1;
					fs.insert(fs.begin() + j, e);
					break;
				}
			}
		}
	}

	int j = 0;
	for (int i = 0; i < fs.size(); i++) {
		
		for (int k = 0; k < fs[i].width; k++) {
			if (fs[i].file) {
				p2 += j * (fs[i].id);
			}
			j++;
		}
	}

	default_timer.end(2);

	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

	default_timer.display_all();
}


//Part 1: 6399153661894
//Part 2 : 6421724645083
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 581.9 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 208.9 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 102833.7 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 224
//Ticks : 2245607
//TotalDays : 2.59908217592593E-06
//TotalHours : 6.23779722222222E-05
//TotalMinutes : 0.00374267833333333
//TotalSeconds : 0.2245607
//TotalMilliseconds : 224.5607