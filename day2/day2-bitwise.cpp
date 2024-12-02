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

void print_decimal(const uint64_t& n) {
    for (int i = 7; i > 0; i--) {
		uint8_t byte = static_cast<uint8_t>((n >> (i * 8)) & 0xFF);
		std::cout << static_cast<int>(byte) << ' ';
    }
	std::cout << '\n';
}

bool is_safe(uint64_t value) {
    bool increasing = true;
    bool decreasing = true;

    for (int i = 7; i > 0; --i) {
        // Extract two consecutive bytes
        uint8_t curr = static_cast<uint8_t>((value >> (i * 8)) & 0xFF);
        uint8_t prev = static_cast<uint8_t>((value >> ((i - 1) * 8)) & 0xFF);

        // Exit early if either byte is zero
        if (curr == 0 || prev == 0) {
            break;
        }

        // Calculate difference without std::abs
        int diff = static_cast<int>(curr) - static_cast<int>(prev);

        // Check bounds and monotonicity in a single branch
        if (diff == 0 || diff > 3 || diff < -3) {
            return false;
        }
        increasing &= (diff > 0);
        decreasing &= (diff < 0);
    }

    return increasing || decreasing;
}


int main() {

    aoc_utils::Timer t;

    aoc_utils::timer_config input_timer_config{
        .id = 0,
        .units = "microseconds",
        .label = "Input",
        .description = "Read input from file and parse into uint64_t",
    };

    aoc_utils::timer_config p1_timer_config{
        .id = 1,
        .units = "microseconds",
        .label = "Part 1",
        .description = "Compute part 1",
    };

    aoc_utils::timer_config p2_timer_config{
        .id = 2,
        .units = "microseconds",
        .label = "Part 2",
        .description = "Compute part 2",
    };


	t.create_timer(input_timer_config);
    t.create_timer(p1_timer_config);
    t.create_timer(p2_timer_config);

	t.begin(0);

	std::vector<uint64_t> in = aoc_utils::read_lines_mmap<uint64_t>("input.txt", [&](const std::string& line) {
		std::vector<uint32_t> ns = aoc_utils::tokenize<uint32_t>(line, " ", [](const std::string& s) { return std::stoull(s); });
		uint64_t v = 0;
		int i = 64 - 8;
		uint64_t d = 0, d2 = 0;

		for (const uint64_t& t : ns) {
			d2 = d;
			d = t << i;
			i -= 8;
			v |= d;
		}

		return v;
		});

	t.end(0);


    t.begin(1);
	int p1 = std::reduce(std::execution::seq, in.begin(), in.end(), 0, [](int acc, const uint64_t& n) { return acc + is_safe(n); });
    t.end(1);

	std::cout << "PART 1: " << p1 << '\n';



	t.begin(2);

    int p2 = std::reduce(std::execution::seq, in.begin(), in.end(), 0, [](int acc, uint64_t v) {
        if (is_safe(v)) {
            return acc + 1;
        }

        for (int i = 0; i < 8; ++i) {
            uint8_t current = static_cast<uint8_t>((v >> ((7 - i) * 8)) & 0xFF);

            uint64_t modified_v = 0;
            for (int j = 0, shift = 0; j < 8; ++j) {
                if (j != i) {
                    uint8_t number = static_cast<uint8_t>((v >> ((7 - j) * 8)) & 0xFF);
                    modified_v |= static_cast<uint64_t>(number) << ((7 - shift) * 8);
                    shift++;
                }
            }

            if (is_safe(modified_v)) {
                return acc + 1;
            }
        }

        return acc;
        });

	t.end(2);


    std::cout << "PART 2: " << p2 << '\n';




    t.display_all();

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