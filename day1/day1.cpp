#include "../utils/utils.h"

int main() {
    std::vector<int> a, b;

	uint8_t* m_arr = (uint8_t*)calloc(2 << 17, sizeof(uint8_t));

	aoc_utils::read_lines_mmap_apply<int>("input.txt", [&](const std::string& line) {
		std::stringstream ss(line);
		std::pair<int, int> p;
		ss >> p.first >> p.second;
		a.emplace_back(p.first);
		b.emplace_back(p.second);
		m_arr[p.second]++;
		return 0;
		});
	
	std::sort(std::execution::par, a.begin(), a.end());
	std::sort(std::execution::par, b.begin(), b.end());

	int p1 = std::transform_reduce(std::execution::par, a.begin(), a.end(), b.begin(), 0, std::plus<>(), [](int a, int b) { return std::abs(a - b); });

	//int p2 = std::reduce(std::execution::par, a.begin(), a.end(), 0, [&m_arr](int acc, int v) { return acc + v * m_arr[v]; });
	int p2 = std::reduce(std::execution::seq, a.begin(), a.end(), 0, [&m_arr](int acc, int v) { return acc + v * m_arr[v]; });

	/*int p2 = 0;
	for (int v : a) {
		p2 += v * m_arr[v];
	}*/

	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';

	free(m_arr);

}

//Part 1: 1530215
//Part 2 : 26800609
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 24
//Ticks : 247739
//TotalDays : 2.86734953703704E-07
//TotalHours : 6.88163888888889E-06
//TotalMinutes : 0.000412898333333333
//TotalSeconds : 0.0247739
//TotalMilliseconds : 24.7739