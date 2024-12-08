#include "../utils/utils.h"

#include <set>

int main() {
	
	INITIALIZE_AOC_TIMERS();


	std::map<char, std::vector<std::pair<int, int>>> m;

	bool first = true;
	int width = 0, height = 0;


	default_timer.begin(0);
	std::string_view sv;
	boost::iostreams::mapped_file_source file;
	aoc_utils::memory_map_file_boost_sv("input.txt", file, sv);

	for (const char c : sv) {
		if (c == '\n') {
			first = false;
			height++;
			width = 0;
			continue;
		}
		if (c != '.')
			m[c].push_back({ width, height });
		width++;
	}
	height++;

	default_timer.end(0);
	
	int p1 = 0, p2 = 0;

	default_timer.begin(1);
	std::vector<uint8_t> an(width * height, 0);
	for (const auto& [key, value] : m) {
		for (int i = 0; i < value.size() - 1; i++) {
			for (int j = i + 1; j < value.size(); j++) {
				int dx = value[i].first - value[j].first;
				int dy = value[i].second - value[j].second;

				if (value[i].first + dx >= 0 && value[i].first + dx < width && value[i].second + dy >= 0 && value[i].second + dy < height)
					an[value[i].first + dx + (value[i].second + dy) * width] = 1;
				if (value[j].first - dx >= 0 && value[j].first - dx < width && value[j].second - dy >= 0 && value[j].second - dy < height)
					an[value[j].first - dx + (value[j].second - dy) * width] = 1;

			}
		}
	}

	p1 = std::reduce(std::execution::unseq, an.begin(), an.end(), 0);

	default_timer.end(1);

	default_timer.begin(2);

	
	std::vector<uint8_t> an2(width * height, 0);

	for (const auto& [key, value] : m) {
		for (int i = 0; i < value.size() - 1; i++) {
			for (int j = i + 1; j < value.size(); j++) {
				int dx = value[j].first - value[i].first;
				int dy = value[j].second - value[i].second;

				int mult = 0;
				while (true) {
					int new_x = value[i].first + dx * mult;
					int new_y = value[i].second + dy * mult;
					if (new_x >= width || new_x < 0 || new_y >= height || new_y < 0)
						break;
					an2[new_y * width + new_x] = 1;
					mult++;
				}

				mult = 0;
				while (true) {
					int new_x = value[j].first - dx * mult;
					int new_y = value[j].second - dy * mult;
					if (new_x >= width || new_x < 0 || new_y >= height || new_y < 0)
						break;
					an2[new_y * width + new_x] = 1;
					mult++;
				}
			}
		}
	}
	p2 = std::reduce(std::execution::unseq, an2.begin(), an2.end(), 0);
	default_timer.end(2);

	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

	default_timer.display_all();
}

//Part 1: 367
//Part 2 : 1285
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 97.3 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 8.5 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 11.9 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 15
//Ticks : 159110
//TotalDays : 1.84155092592593E-07
//TotalHours : 4.41972222222222E-06
//TotalMinutes : 0.000265183333333333
//TotalSeconds : 0.015911
//TotalMilliseconds : 15.911