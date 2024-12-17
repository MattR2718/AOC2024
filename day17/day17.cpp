#include "utils.h"

struct Registers {
	int64_t A = 0;
	int64_t B = 0;
	int64_t C = 0;

	void print() const {
		fmt::print("Register A: {}\n", A);
		fmt::print("Register B: {}\n", B);
		fmt::print("Register C: {}\n", C);
	}
};

std::string apply_instruction(Registers& registers, int opcode, int operand, int& ip) {
	switch (opcode) {
	case 0: { // adv (integer division)

		int64_t numer = registers.A;

		if (operand <= 3) {
			registers.A = numer >> operand;
			//fmt::print("1: Dividing register A by 2 ^ {}\n", operand);
		}
		else if (operand == 4) {
			registers.A = numer >> numer;
			//fmt::print("2: Dividing register A by 2 ^ A\n");
		}
		else if (operand == 5) {
			registers.A = numer >> registers.B;
			//fmt::print("3: Dividing register A by 2 ^ B\n");
		}
		else if (operand == 6) {
			registers.A = numer >> registers.C;
			//fmt::print("4: Dividing register A by 2 ^ C\n");
		}
		else {
			//fmt::print("Invalid operand: {}, Opcode {}\n", operand, opcode);
		}

		break;
	}
	case 1: { // bxl
		registers.B = registers.B ^ operand;
		//fmt::print("5: XORing register B with {}\n", operand);
		break;
	}
	case 2: { // bst
		if (operand <= 3) {
			registers.B = operand % 8;
			//fmt::print("6: Setting register B to {}\n", operand % 8);
		}
		else if (operand == 4) {
			registers.B = registers.A % 8;
			//fmt::print("7: Setting register B to A % 8\n");
		}
		else if (operand == 5) {
			registers.B = registers.B % 8;
			//fmt::print("8: Setting register B to B % 8\n");
		}
		else if (operand == 6) {
			registers.B = registers.C % 8;
			//fmt::print("9: Setting register B to C % 8\n");
		}
		else {
			//fmt::print("Invalid operand: {}, Opcode {}\n", operand, opcode);
		}
		break;
	}
	case 3: { // jnz
		if (registers.A) {
			ip = operand - 2; // -2 because we increment the ip by 2 at the end of the loop
			//fmt::print("10: Jumping to Start\n");
		}
		break;
	}
	case 4: { // bxc
		registers.B = registers.B ^ registers.C;
		//fmt::print("11: XORing register B with register C\n");
		break;
	}
	case 5: { // out
		int out = -1;
		if (operand <= 3) {
			out = operand % 8;
			//fmt::print("12.1: Output: {}\n\n", out);

		}
		else if (operand == 4) {
			out = registers.A % 8;
			//fmt::print("12.2: Output: A % 8\n\n");

		}
		else if (operand == 5) {
			out = registers.B % 8;
			//fmt::print("12.3: Output: B % 8\n\n");

		}
		else if (operand == 6) {
			out = registers.C % 8;
			//fmt::print("12.4: Output: C % 8\n\n");

		}
		else {
			//fmt::print("Invalid operand: {}, Opcode {}\n", operand, opcode);
		}

		////fmt::print("{},", out);


		return std::to_string(out) + ",";

		break;
	}
	case 6: { // bdv
		int64_t numer = registers.A;

		if (operand <= 3) {
			registers.B = numer >> operand;
			//fmt::print("13: Setting register B to register A >> 2 ^ {}\n", operand);
		}
		else if (operand == 4) {
			registers.B = numer >> numer;
			//fmt::print("14: Setting register B to register A >> A\n");
		}
		else if (operand == 5) {
			registers.B = numer >> registers.B;
			//fmt::print("15: Setting register B to register A >> B\n");
		}
		else if (operand == 6) {
			registers.B = numer >> registers.C;
			//fmt::print("16: Setting register B to register A >> C\n");
		}
		else {
			//fmt::print("Invalid operand: {}, Opcode {}\n", operand, opcode);
		}
		break;
	}
	case 7: { // cdv
		int64_t numer = registers.A;

		if (operand <= 3) {
			registers.C = numer >> operand;
			//fmt::print("17: Setting register C to register A >> 2 ^ {}\n", operand);
		}
		else if (operand == 4) {
			registers.C = numer >> numer;
			//fmt::print("18: Setting register C to register A >> A\n");
		}
		else if (operand == 5) {
			registers.C = numer >> registers.B;
			//fmt::print("19: Setting register C to register A >> B\n");
		}
		else if (operand == 6) {
			registers.C = numer >> registers.C;
			//fmt::print("20: Setting register C to register A >> C\n");
		}
		else {
			//fmt::print("Invalid operand: {}, Opcode {}\n", operand, opcode);
		}
		break;
	}
	}

	return "";
}


bool tryA(uint64_t A, const std::vector<int>& expected) {
	uint64_t B = 0;
	uint64_t C = 0;

	for (size_t i = 0; i < expected.size(); i++) {
		// Step 7: Setting B to A % 8
		B = A % 8ull;

		// Step 5: XORing B with 2
		B = B ^ 2ull;

		// Step 19: Setting C to A >> B
		C = A >> B;

		// Step 1: Dividing A by 2^3 (equivalent to A >> 3)
		A = A >> 3ull;

		// Step 11: XORing B with C
		B = B ^ C;

		// Step 5: XORing B with 7
		B = B ^ 7ull;

		// Step 12.3: Output B % 8 and compare with expected[i]
		uint64_t output = B % 8ull;
		//std::cout << "Step " << i + 1 << ": B = " << B << ", Output = " << output << ", Expected = " << expected[i] << "\n";

		if (output != expected[i]) {
			return false; // Mismatch: return false immediately
		}
	}

	return true; // All outputs matched
}



void check_option(const std::vector<int>& program, uint64_t val) {

	Registers registers;
	registers.A = val;
	registers.B = 0;
	registers.C = 0;

	std::string p1 = "";

	for (int i = 0; i < program.size() - 1; i += 2) {
		auto opcode = program[i];
		auto operand = program[i + 1];

		p1 += apply_instruction(registers, opcode, operand, i);
	}

	//fmt::print("VAL: {}, OUTPUT: {}\n", val, p1);

	if(p1 == "2,4,1,2,7,5,0,3,4,7,1,7,5,5,3,0,"){
		fmt::print("\n\n\n\n\n\n\n\n\n");
		fmt::print("\n\n\n\n\n\n\n\n\n");
		fmt::print("\n\n\n\n\n\n\n\n\n");
		fmt::print("\n\n\n\n\n\n\n\n\n");
		fmt::print("=================================================Found: {}\n", val);
		fmt::print("\n\n\n\n\n\n\n\n\n");
		fmt::print("\n\n\n\n\n\n\n\n\n");
		fmt::print("\n\n\n\n\n\n\n\n\n");
		fmt::print("\n\n\n\n\n\n\n\n\n");
		fmt::print("\n\n\n\n\n\n\n\n\n");

	
	}
}



void generate_combinations(const std::vector<std::vector<int>>& valid_solutions, int index, int64_t current_value, const std::vector<int>& program) {
	// Base case: if we've processed all indices, print the current combination (int64_t value)
	if (index == valid_solutions.size()) {
		//std::cout << "Generated value: " << current_value << std::endl;
		check_option(program, current_value);
		return;
	}

	// Recursive case: iterate over all valid solutions at the current index
	for (int i : valid_solutions[index]) {
		// For each valid value i at index, shift it to its correct position in the result
		int64_t new_value = current_value | (static_cast<int64_t>(i) << (index * 3));  // 3 is the shift factor, adjust if needed
		generate_combinations(valid_solutions, index + 1, new_value, program);  // Recur for the next index
	}
}









int main() {
	Registers registers;
	std::vector<int> program;

	int l = 0;
	aoc_utils::read_lines_mmap_apply<int>("input.txt", [&registers, &program, &l](const std::string& line) {
		for (auto& [m, n] : ctre::search_all<R"((\d+))">(line)) {
			if (l == 0) {
				registers.A = n.to_number<int64_t>();
			}else if (l == 1) {
				registers.B = n.to_number<int64_t>();
			}
			else if (l == 2) {
				registers.C = n.to_number<int64_t>();
			}
			else if (l == 3) {
				return 0;
			}
			else {
				program.push_back(n.to_number<int>());
			}
		}
		l++;

		return 0;
	});
	/*registers.print();

	for (auto ins : program) {
		fmt::print("Instruction: {}\n", ins);
	}*/

	std::string p1 = "";
	uint64_t p2 = 0;

	// Part 1
	for (int i = 0; i < program.size() - 1; i += 2) {
		auto opcode = program[i];
		auto operand = program[i + 1];

		p1 += apply_instruction(registers, opcode, operand, i);
	}

	/*std::string p2_goal = "";
	for(auto& ins : program){
		p2_goal += std::to_string(ins) + ",";
	}*/

	//std::cout<<UINT64_MAX<<std::endl;

	//for(int i = 0; i < INT_MAX; i++){
	//	if(i % 1000000 == 0){
	//		fmt::print("Trying: {}\n", i);
	//	}
	//	registers.A = i;
	//	registers.B = 0;
	//	registers.C = 0;

	//	std::string p2_str = "";
	//	for (int i = 0; i < program.size() - 1; i += 2) {
	//		auto opcode = program[i];
	//		auto operand = program[i + 1];

	//		p2_str += apply_instruction(registers, opcode, operand, i);
	//	}

	//	if(p2_str == p2_goal){
	//		p2 = i;
	//		break;
	//	}

	//	//fmt::print("Part 2: {}\n", p2_str);
	//	//p2 = "";
	//}
	std::vector<int> expected = aoc_utils::tokenize<int>(p1, ",", [](const std::string& s) { return std::stoi(s); });

	std::vector<std::vector<int>> valid_solutions(program.size());


	for (uint64_t j = 0; j < program.size(); j++) {
		int goal = program[j];
		bool found_valid_solution = false;

		for (uint64_t i = 0; i < (1ull << 9ull); i++) {
			// Fixed operator precedence with parentheses
			if ((((((i % 8) ^ 2) ^ (i >> ((i % 8) ^ 2))) ^ 7) % 8) == goal) {
				std::cout << "Valid: " << i << '\n';
				valid_solutions[j].push_back(i);
				found_valid_solution = true;
			}
			else {
				std::cout << "NOT " << i << '\n';
			}
		}

		if (!found_valid_solution) {
			std::cout << "No valid solution found for index " << j << "\n";
		}

		std::cout << "Index: " << j << "\n\n\n";
	}


	int64_t initial_value = 0;
	generate_combinations(valid_solutions, 0, initial_value, program);


	/*for (uint64_t j = 0; j < program.size(); j++) {
		int goal = program[j];
		for (uint64_t i = 0; i < 8; i++) {
			if(((((i % 8) ^ 2) ^ (i >> ((i % 8)^2)))^7)%8 == goal){
				std::cout << i << '\n';
				p2 |= i << j * 3ll;
				break; 
			}
			else {
				std::cout << "NOT " << i << '\n';
			}
		}
		std::cout << "Index: " << j <<"\n\n\n";
	}*/












	/*for (uint64_t i = 0; i < INT64_MAX; i++) {
		if (i % 1000000 == 0) {
			fmt::print("Trying: {}\n", i);
		}
	}*/

	/*for (int i = 0; i < 2 << 6; i++) {
		fmt::print("A % 8 = {}, (A % 8) xor 2 = {}\n", i % 8, (i % 8) ^ 2);
	}*/

	//std::vector<int> expected = aoc_utils::tokenize<int>(p1, ",", [](const std::string& s) { return std::stoi(s); });
	/*
	for (uint64_t A = 265126244322205; A > 0; A--) {
		if(A % 100000000 == 0){
			fmt::print("Trying: {}\n", A);
		}
		if (tryA(A, expected)) {
			std::cout << "High: " << A << '\n';
			p2 = A;
			break;
		}
	}*/



	tryA(190384113204239, expected);

	std::cout << "Part 1: " << p1.substr(0, p1.length() - 1) << '\n' << "Part 2: " << p2 << '\n';

}

// High: 18446744071622753469
// High: 265,126,244,322,205
// 
// Not:  202,340,637,622,031
// 
// Actu: 190,384,113,204,239