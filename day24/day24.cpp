#include "../utils/utils.h"

#include <functional>
#include <algorithm>

#include "draw_circuit.h"

struct wire{
	std::string label;
	bool val;
	bool value_been_set = false;

};

std::ostream& operator<<(std::ostream& os, const wire& w) {
	os << "(" << w.label << " " << (w.value_been_set ? std::to_string(w.val) : "N/A") << ")";
	return os;
}

struct gate {
	wire* in1;
	wire* in2;
	wire* out;
	std::function<bool(bool, bool)> op;
};

std::ostream& operator<<(std::ostream& os, const gate& g) {
	if (g.op(1, 1) && g.op(1, 0)) {
		os << "OR: \n";
	}
	else if (g.op(1, 1)) {
		os << "AND: \n";
	}
	else {
		os << "XOR: \n";
	}

	os << "\t" << * g.in1 << " " << *g.in2 << " " << *g.out;
	return os;
}

int main() {
	std::map<std::string, wire> wires;
    std::vector<gate>gates;

	bool wires_in = true;
	int num_each_wire = 0;
	aoc_utils::read_lines_mmap_apply<void>("input.txt", [&](const std::string& line) {
		if (line.empty()) { wires_in = false; return; }
		if (wires_in) {
			auto match = ctre::match<R"((.+): ([10]))">(line);
			if (match) {
				wire w;
				w.label = match.get<1>().to_string();
				w.val = match.get<2>() == "1";
				w.value_been_set = true;
				wires[w.label] = w;
			}
		}
		else {
			if (num_each_wire == 0) {
				num_each_wire = wires.size() / 2;
			}

			auto match = ctre::match<R"((.+) (AND|OR|XOR) (.+) -> (.+))">(line);
			if (match) {
				std::string w1 = match.get<1>().to_string();
				std::string w2 = match.get<3>().to_string();
				std::string out = match.get<4>().to_string();
				if (wires.count(w1) == 0) {
					wire w;
					w.label = w1;
					w.value_been_set = false;
					wires[w1] = w;
				}
				if (wires.count(w2) == 0) {
					wire w;
					w.label = w2;
					w.value_been_set = false;
					wires[w2] = w;
				}
				if (wires.count(out) == 0) {
					wire w;
					w.label = out;
					w.value_been_set = false;
					wires[out] = w;
				}
				gate g;
				g.in1 = &wires[w1];
				g.in2 = &wires[w2];
				g.out = &wires[out];
				if (match.get<2>() == "AND") {
					//g.op = [](bool a, bool b) {return a && b; };
					g.op = std::bit_and<bool>();
				}
				else if (match.get<2>() == "OR") {
					//g.op = [](bool a, bool b) {return a || b; };
					g.op = std::bit_or<bool>();
				}
				else if (match.get<2>() == "XOR") {
					//g.op = [](bool a, bool b) {return a ^ b; };
					g.op = std::bit_xor<bool>();
				}
				gates.push_back(g);
			}

		}

	});
	
	for (const auto& g : gates) {
		std::cout << g << '\n';
	}

	bool all_set = true;
	do {
		all_set = true;
		for (auto& g : gates) {
			if (g.in1->value_been_set && g.in2->value_been_set && g.out->value_been_set) {
				continue;
			}
			if (!g.in1->value_been_set || !g.in2->value_been_set) {
				all_set = false;
				continue;
			}
			g.out->val = g.op(g.in1->val, g.in2->val);
			g.out->value_been_set = true;
		}
	} while (!all_set);


	std::cout << "\n\n\n";

	for (const auto& g : gates) {
		std::cout << g << '\n';
	}

	uint64_t p1 = 0;
	uint64_t i = 0;
	for (const auto& [k, v] : wires) {
		if (k.front() == 'z') {
			std::cout << k << ": " << v.val << '\n';
			p1 |= (static_cast<uint64_t>(v.val) << i++);
		}
	}


	uint64_t p2 = 0;

	std::vector<gate_node> gate_nodes;

	for (const auto& g : gates) {
		gate_node gn;
		gn.in1 = g.in1->label;
		gn.in2 = g.in2->label;
		gn.out = g.out->label;
		//gn.op = g.op == std::bit_and<bool>() ? "AND" : g.op == std::bit_or<bool>() ? "OR" : "XOR";
		
		if (g.op(1, 1) && g.op(1, 0)) {
			gn.op = "OR";
		}
		else if (g.op(1, 1)) {
			gn.op = "AND";
		}
		else {
			gn.op = "XOR";
		}
		
		gate_nodes.push_back(gn);
	}

	draw_diagram(num_each_wire, gate_nodes);

	//run_example();

	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2 << '\n';


}


// P1: 48508229772400