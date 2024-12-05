#include "../utils/utils.h"


int lr = 0, ud = 0, urdl = 0, uldr = 0;

int check_xy(std::vector<std::string> &a, int i, int j) {

	std::string_view w = "XMAS";
	int n = 0;
	// Right
	int i2 = i;
	while (i2 < a[j].length()) {
		if (a[j][i2] == w[i2 - i]) {
			i2++;
		}
		else {
			break;
		}

		if (i2 - i == 4) {
			n++;
			lr++;
			break;
		}
	}

	// Left
	i2 = i;
	while (i2 > -1) {
		if (a[j][i2] == w[i - i2]) {
			i2--;
		}
		else {
			break;
		}

		if (i - i2 == 4) {
			n++;
			lr++;
			break;
		}
	}

	// Down
	int j2 = j;
	while (j2 < a.size()) {
		if (a[j2][i] == w[j2 - j]) {
			j2++;
		}
		else {
			break;
		}

		if (j2 - j == 4) {
			n++;
			ud++;
			break;
		}
	}

	// Left
	j2 = j;
	while (j2 > -1) {
		if (a[j2][i] == w[j - j2]) {
			j2--;
		}
		else {
			break;
		}

		if (j - j2 == 4) {
			n++;
			ud++;
			break;
		}
	}

	// Down Right
	i2 = i;
	j2 = j;
	while (i2 < a[j].length() && j2 < a.size()) {
		if (a[j2][i2] == w[j2 - j]) {
			i2++;
			j2++;
		}
		else {
			break;
		}

		if (j2 - j == 4) {
			n++;
			uldr++;
			break;
		}
	}

	// Down Left
	i2 = i;
	j2 = j;
	while (i2 > -1 && j2 < a.size()) {
		if (a[j2][i2] == w[j2 - j]) {
			i2--;
			j2++;
		}
		else {
			break;
		}

		if (j2 - j == 4) {
			n++;
			urdl++;
			break;
		}
	}

	// Up Right
	i2 = i;
	j2 = j;
	while (i2 < a[j].length() && j2 > -1) {
		if (a[j2][i2] == w[j - j2]) {
			i2++;
			j2--;
		}
		else {
			break;
		}

		if (j - j2 == 4) {
			n++;
			urdl++;
			break;
		}
	}

	// Up Left
	i2 = i;
	j2 = j;
	while (i2 > -1 && j2 > -1) {
		if (a[j2][i2] == w[j - j2]) {
			i2--;
			j2--;
		}
		else {
			break;
		}

		if (j - j2 == 4) {
			n++;
			uldr++;
			break;
		}
	}

	return n;
}

int mas_x(std::vector<std::string>& a, int i, int j) {
	int n = 0;
	if(i - 1 > -1 && j - 1 > -1 && a[j - 1][i - 1] == 'M' && i + 1  < a[i].length() && j + 1 < a.size() && a[j + 1][i + 1] == 'S' || i - 1 > -1 && j - 1 > -1 && a[j - 1][i - 1] == 'S' && i + 1 < a[i].length() && j + 1 < a.size() && a[j + 1][i + 1] == 'M') {
		n++;
	}

	if (i - 1 > -1 && j + 1 < a.size() && a[j + 1][i - 1] == 'M' && i + 1 < a[i].length() && j - 1 > -1&& a[j - 1][i + 1] == 'S' || i - 1 > -1 && j + 1 < a.size() && a[j + 1][i - 1] == 'S' && i + 1 < a[i].length() && j - 1 > -1 && a[j - 1][i + 1] == 'M') {
		n++;
	}

	return n == 2 ? 1 : 0;

}

int main() {

	INITIALIZE_AOC_TIMERS();

	default_timer.begin(0);
    std::vector<std::string> a = aoc_utils::read_lines_mmap<std::string>("input.txt");
	default_timer.end(0);

    default_timer.begin(1);
	int p1 = 0;
	int p2 = 0;
	for (int j = 0; j < a.size(); j++) {
		for (int i = 0; i < a[j].length(); i++) {
			if (a[j][i] == 'X') {
				p1 += check_xy(a, i, j);
			}
			else if (a[j][i] == 'A') {
				p2 += mas_x(a, i, j);
			}
		}
	}
	default_timer.end(1);


	std::cout << "Part 1: " << p1 << "\n";
	std::cout<<"Part 2: "<<p2<<"\n";
	default_timer.display_all();

	std::cout << lr << '\n' << ud << '\n' << uldr << '\n' << urdl << '\n';
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