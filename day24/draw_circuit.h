#include "imgui.h"
#include "imgui-node-editor/imgui_node_editor.h"
#include "hello_imgui/hello_imgui.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

#include <set>

namespace ed = ax::NodeEditor;

struct gate_node {
    std::string in1, in2;
    std::string out;
    std::string op;  // Operation (AND, OR, XOR, etc.)
    ed::NodeId id;   // Node ID for visualization
};


static ed::EditorContext* g_Context = nullptr;
static int g_NextId = 1;

// Custom hash function for NodeId and PinId
struct NodeIdHasher {
    std::size_t operator()(const ed::NodeId& id) const {
        return std::hash<void*>{}(id.AsPointer());
    }
};

struct PinIdHasher {
    std::size_t operator()(const ed::PinId& id) const {
        return std::hash<void*>{}(id.AsPointer());
    }
};

// Helper to get or create wire IDs
std::unordered_map<std::string, ed::PinId, std::hash<std::string>> wirePins;
std::unordered_map<std::string, ed::NodeId, std::hash<std::string>> inputNodes;
std::unordered_map<ed::NodeId, ImVec2, NodeIdHasher> nodePositions;

std::vector<std::pair<std::string, int>> y_values;


ed::PinId GetWirePin(const std::string& wireLabel) {
    if (wirePins.find(wireLabel) == wirePins.end()) {
        wirePins[wireLabel] = ed::PinId(g_NextId++);
    }
    return wirePins[wireLabel];
}

ed::NodeId GetInputNodeId(const std::string& inputLabel) {
    if (inputNodes.find(inputLabel) == inputNodes.end()) {
        inputNodes[inputLabel] = ed::NodeId(g_NextId++);
    }
    return inputNodes[inputLabel];
}

void RenderCircuitDiagram(const std::vector<std::pair<std::string, std::string>>& inputs,
    const std::vector<gate_node>& gates) {
    if (!g_Context) {
        g_Context = ed::CreateEditor();
    }

    ed::SetCurrentEditor(g_Context);
    ed::Begin("Logic Circuit Diagram");

    float nodeSpacingX = 500.0f;
    float nodeSpacingY = 200.0f;

    int i = 0;

    // Draw inputs as standalone labels
    for (const auto& input : inputs) {
        ed::NodeId nodeId = GetInputNodeId(input.first);
        ed::BeginNode(nodeId);
        //ImGui::Text("Input %s = %s", input.first.c_str(), input.second.c_str());
        ed::BeginPin(GetWirePin(input.first + "_i"), ed::PinKind::Output);
        ImGui::Text("Out: %s", input.first.c_str());
        ed::EndPin();
        ed::BeginPin(GetWirePin(input.second + "_i"), ed::PinKind::Output);
        ImGui::Text("Out: %s", input.second.c_str());
        ed::EndPin();
        ed::EndNode();

        if (nodePositions.find(nodeId) == nodePositions.end()) {
            nodePositions[nodeId] = ImVec2(0, i * nodeSpacingY * 2);
        }
        ed::SetNodePosition(nodeId, nodePositions[nodeId]);
        i++;
    }

    i = 0;
    // Draw gates
    for (const auto& g : gates) {
        ed::BeginNode(g.id);
        ImGui::Text("%s", g.op.c_str());

        std::string in_suffix = (g.op == "AND") ? "_and" : (g.op == "OR" ? "_or" : "_XOR");
        std::string out_suffix = (g.op == "AND") ? "_and_o" : (g.op == "OR" ? "_or_o" : "_XOR_o");

        // Input pins
        ed::BeginPin(GetWirePin(g.in1 + in_suffix), ed::PinKind::Input);
        ImGui::Text("In1: %s", g.in1.c_str());
        ed::EndPin();

        ed::BeginPin(GetWirePin(g.in2 + in_suffix), ed::PinKind::Input);
        ImGui::Text("In2: %s", g.in2.c_str());
        ed::EndPin();

        // Output pin
        ed::BeginPin(GetWirePin(g.out + out_suffix), ed::PinKind::Output);
        ImGui::Text("Out: %s", g.out.c_str());
        ed::EndPin();

        ed::EndNode();

        if (nodePositions.find(g.id) == nodePositions.end()) {

            // First row
			if (ctre::match<R"((x|y)[0-9]{2})">(g.in1) && ctre::match<R"((x|y)[0-9]{2})">(g.in2)) {
                if (g.op == "XOR") {
                    nodePositions[g.id] = ImVec2(nodeSpacingX, std::stoi(g.in1.substr(1, 2)) * nodeSpacingY * 2);
                }
                else {
                    nodePositions[g.id] = ImVec2(nodeSpacingX, std::stoi(g.in1.substr(1, 2)) * nodeSpacingY * 2 + nodeSpacingY);
                }
				//nodePositions[g.id] = ImVec2(nodeSpacingX, std::stoi(g.in1.substr(1, 2)) * nodeSpacingY);
			}
            else if (ctre::match<R"((z)[0-9]{2})">(g.out)) {
                nodePositions[g.id] = ImVec2(nodeSpacingX * 3, std::stoi(g.out.substr(1, 2)) * nodeSpacingY * 2);
            }            
            else{


                nodePositions[g.id] = ImVec2(nodeSpacingX * -1, i * nodeSpacingY);
            }


            //nodePositions[g.id] = ImVec2((i % 3 + 1) * nodeSpacingX, (i / 3) * nodeSpacingY);
        }
        ed::SetNodePosition(g.id, nodePositions[g.id]);
        i++;
    }

    // Create links from input
	for (const auto& input : inputs) {
		for (const auto& gate : gates) {
			//std::cout << input.first << " " << gate.in1 << std::endl;
			//std::cout << input.second << " " << gate.in1 << std::endl;

            std::string suffix = (gate.op == "AND") ? "_and" : (gate.op == "OR" ? "_or" : "_in");
			
			if (input.first == gate.in1) {
                //std::cout << "Linking " << input.first << " to " << gate.in1 << '\n';
				ed::Link(ed::LinkId(g_NextId++), GetWirePin(input.first + "_i"), GetWirePin(gate.in1 + suffix));
			}
			if (input.second == gate.in1) {
				//std::cout << "Linking " << input.second << " to " << gate.in1 << '\n';
				ed::Link(ed::LinkId(g_NextId++), GetWirePin(input.second + "_i"), GetWirePin(gate.in1 + suffix));
			}
            if (input.first == gate.in2) {
				//std::cout << "Linking " << input.first << " to " << gate.in2 << '\n';
                ed::Link(ed::LinkId(g_NextId++), GetWirePin(input.first + "_i"), GetWirePin(gate.in2 + suffix));
            }
            if (input.second == gate.in2) {
				//std::cout << "Linking " << input.second << " to " << gate.in2 << '\n';
                ed::Link(ed::LinkId(g_NextId++), GetWirePin(input.second + "_i"), GetWirePin(gate.in2 + suffix));
            }
		}
	}

    // Intermediate nodes

    for (const auto& g1 : gates) {
        std::string in_suffix1 = (g1.op == "AND") ? "_and" : (g1.op == "OR" ? "_or" : "_XOR");
        std::string out_suffix1 = (g1.op == "AND") ? "_and_o" : (g1.op == "OR" ? "_or_o" : "_XOR_o");
        for (const auto& g2 : gates) {
            std::string in_suffix2 = (g2.op == "AND") ? "_and" : (g2.op == "OR" ? "_or" : "_XOR");
            std::string out_suffix2 = (g2.op == "AND") ? "_and_o" : (g2.op == "OR" ? "_or_o" : "_XOR_o");

			if (g1.out == g2.in1) {
				//std::cout << "Linking " << g1.out << " to " << g2.in1 << '\n';
				ed::Link(ed::LinkId(g_NextId++), GetWirePin(g1.out + out_suffix1), GetWirePin(g2.in1 + in_suffix2));
			}
            if (g1.out == g2.in2) {
                //std::cout << "Linking " << g1.out << " to " << g2.in2 << '\n';
                ed::Link(ed::LinkId(g_NextId++), GetWirePin(g1.out + out_suffix1), GetWirePin(g2.in2 + in_suffix2));
            }
        }
    }

    ed::End();
    ed::SetCurrentEditor(nullptr);
}

//void calculate_y_values(std::vector<std::pair<std::string, std::string>> inputs, std::vector<gate_node> gates) {
//    std::map<std::string, std::set<gate_node>> part_of;
//    for (int i = 0; i < 5; i++) {
//        for (const auto& gate : gates) {
//            if (ctre::match<R"((x|y)[0-9]{2})">(gate.in1) && ctre::match<R"((x|y)[0-9]{2})">(gate.in2)) {
//                part_of[gate.in1].insert(gate);
//                part_of[gate.in2].insert(gate);
//            }
//        }
//    }
//
//
//}

void calculate_y_values(std::vector<std::pair<std::string, std::string>> inputs, std::vector<gate_node> gates) {
    
    for (const auto& input : inputs) {
		//std::cout << input.first << " " << std::stoi(input.first.substr(1, 2)) * 2 << '\n';
        y_values.push_back(std::make_pair(input.first, std::stoi(input.first.substr(1, 2)) * 2));
        y_values.push_back(std::make_pair(input.second, std::stoi(input.first.substr(1, 2)) * 2));
    }

    for (const auto& g : gates) {
        for (const auto& y : y_values) {
            if (g.in1 == y.first) {
                y_values.push_back(std::make_pair(g.in1, y.second + 1));
            }
            else if (g.in2 == y.first) {
                y_values.push_back(std::make_pair(g.in2, y.second + 1));
            }
            else if (g.out == y.first) {
                y_values.push_back(std::make_pair(g.out, y.second + 1));
            }
        }
    }

	for (const auto& y : y_values) {
        std::cout << y.first << " " << y.second << '\n';
	}

	std::cout << "TOTAL NUMBER OF Y VALUES: " << y_values.size() << '\n';

	std::cin.get();

}


void draw_diagram(int num_inputs, std::vector<gate_node> gates) {

    std::vector<std::pair<std::string, std::string>> inputs;
    for (int i = 0; i < num_inputs; i++) {
        std::string num = std::to_string(i);
        std::string new_num = std::string(2 - std::min(2, (int)num.length()), '0') + num;
        inputs.push_back(std::make_pair("x" + new_num, "y" + new_num ));
    }

	for (auto& g : gates) {
		g.id = ed::NodeId(g_NextId++);
	}

    calculate_y_values(inputs, gates);

	HelloImGui::Run(
		[&] {
			ImGui::Text("Logic Circuit Diagram");
			RenderCircuitDiagram(inputs, gates);
		},
		"Logic Circuit Diagram", true);
}
