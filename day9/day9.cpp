#include "../utils/utils.h"

#include <set>

struct entry {
	bool file;
	int id = -1;
	int width;
};

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

	for (int i = in.size() - 1; i > 0; i--) {
		if (in[i] != 0) {
			for (int j = 0; j < i; j++) {
				if (in[j] == 0) {
					in[j] = in[i];
					in[i] = 0;

					break;
				}
			}
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
