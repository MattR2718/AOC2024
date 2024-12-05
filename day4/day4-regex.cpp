#include "../utils/utils.h"

#include <regex>

#define COUNT_PATTERN(pattern) [&]{ \
    int count = 0; \
    for (auto _ : ctre::range<pattern>(in)) count++; \
    return count; \
}()

#define TEST false


void print_w_matches(const std::string& s, const std::vector<int>& v) {
	for (int i = 0; i < s.length(); i++) {
		if (std::find(v.begin(), v.end(), i) != v.end()) {
			fmt::print(fmt::fg(fmt::color::green), "{}", s[i]);
		}
		else {
			fmt::print("{}", s[i]);
		}

		if(s[i] == '|'){
			fmt::print("\n");
		}
	}
}


int get_num_matches(const std::string& s, const std::regex& pattern) {
	auto matches_begin = std::sregex_iterator(s.begin(), s.end(), pattern);
	auto matches_end = std::sregex_iterator();

	/*std::vector<int> match_pos;

	for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
		const std::smatch& match = *i;
		std::cout << "Match at position: " << match.position() << '\n';
		match_pos.push_back(match.position());
	}

	print_w_matches(s, match_pos);*/

	int n = std::distance(matches_begin, matches_end);;
	std::cout<<n<<'\n';

	return n;
}

int countMatches(const std::string& input, const std::regex& pattern) {
	std::smatch match;
	int count = 0;
	std::string::const_iterator searchStart = input.begin();
	while (std::regex_search(searchStart, input.end(), match, pattern)) {
		++count;
		searchStart = match.suffix().first + 1; // Move past the current match for overlapping matches
	}
	return count;
}


int main() {

	INITIALIZE_AOC_TIMERS();

	default_timer.begin(0);

	std::string in{ "" };
#if TEST
	aoc_utils::read_lines_mmap_apply<void>("test.txt", [&in](const std::string& s) { in += s + "|"; });
#else
	aoc_utils::read_lines_mmap_apply<void>("input.txt", [&in](const std::string& s) { in += s + "|"; });
#endif
	default_timer.end(0);

	std::cout << in << '\n';

	default_timer.begin(1);
	
#if TEST
	//std::regex pattern1(R"((?=(XMAS|SAMX|X.{10}M.{10}A.{10}S|S.{10}A.{10}M.{10}X|X.{11}M.{11}A.{11}S|S.{11}A.{11}M.{11}X)))");
	//std::regex pattern2(R"((?=(X.{9}M.{9}A.{9}S|S.{9}A.{9}M.{9}X)))");

	std::array<std::regex, 4> patterns = {
		std::regex(R"((?=(XMAS|SAMX)))"),
		std::regex(R"((?=(X.{10}M.{10}A.{10}S|S.{10}A.{10}M.{10}X)))"),
		std::regex(R"((?=(X.{11}M.{11}A.{11}S|S.{11}A.{11}M.{11}X)))"),
		std::regex(R"((?=(X.{9}M.{9}A.{9}S|S.{9}A.{9}M.{9}X)))")
	};
#else
	std::array<std::regex, 4> patterns = {
		std::regex(R"((?=(XMAS|SAMX)))"),
		std::regex(R"((?=(X.{150}M.{150}A.{150}S|S.{150}A.{150}M.{150}X)))"),
		std::regex(R"((?=(X.{151}M.{151}A.{151}S|S.{151}A.{151}M.{151}X)))"),
		std::regex(R"((?=(X.{149}M.{149}A.{149}S|S.{149}A.{149}M.{149}X)))")
	};

#endif

	int p1 = 0;
	for (const auto& p : patterns) {
		p1 += get_num_matches(in, p);
	}

	for (int i = 0; i < patterns.size(); ++i) {
		int count = countMatches(in, patterns[i]);
		std::cout << "Pattern " << i + 1 << " matches " << count << " times." << std::endl;
	}


	int p2 = 0;


	default_timer.end(1);


	std::cout << "Part 1: " << p1 << "\n";
	std::cout << "Part 2: " << p2 << "\n";
	default_timer.display_all();
}

//Part 1: 2549
//Part 2 : 2003
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse into uint64_t
//Elapsed Time : 402.4 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1 + 2
//Description : Compute part 1 + 2
//Elapsed Time : 889.6 microseconds
//========================================================================== =
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 22
//Ticks : 222190
//TotalDays : 2.57164351851852E-07
//TotalHours : 6.17194444444444E-06
//TotalMinutes : 0.000370316666666667
//TotalSeconds : 0.022219
//TotalMilliseconds : 22.219