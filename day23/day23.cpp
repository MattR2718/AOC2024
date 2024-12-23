#include "../utils/utils.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/bron_kerbosch_all_cliques.hpp>

// Graph with string vertex labels
typedef boost::property<boost::vertex_name_t, std::string> VertexProperty;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexProperty> Graph;

// Mapping for vertex descriptors by name
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

// Make sure V is consistent with boost::graph_traits
using V = boost::graph_traits<Graph>::vertex_descriptor;
// Define Clique using std::vector with correct allocator
using Clique = std::vector<V, std::allocator<V>>;
using Cliques = std::vector<Clique>;


template <typename T>
std::vector<std::vector<T>> combinations(const std::vector<T>& elements, int k) {
	std::vector<std::vector<T>> result;
	std::vector<int> indices(k);
	int n = elements.size();

	// Initialize indices
	for (int i = 0; i < k; ++i) {
		indices[i] = i;
	}

	while (true) {
		std::vector<T> subset;
		for (int i : indices) {
			subset.push_back(elements[i]);
		}
		result.push_back(subset);

		// Generate next combination
		int i = k - 1;
		while (i >= 0 && indices[i] == i + n - k) {
			--i;
		}
		if (i < 0) break;

		++indices[i];
		for (int j = i + 1; j < k; ++j) {
			indices[j] = indices[j - 1] + 1;
		}
	}

	return result;
}


struct Collector {
	Cliques& target;

	// Match the exact signature that boost::bron_kerbosch_all_cliques expects
	template <typename VertexVector>
	void clique(const VertexVector& clique, const Graph& g) const {
		if (clique.size() >= 3) {
			// Convert to std::vector for combinations function
			std::vector<V> clique_vec(clique.begin(), clique.end());
			auto sub_cliques = combinations(clique_vec, 3);
			target.insert(target.end(), sub_cliques.begin(), sub_cliques.end());
		}
	}
};


struct Collector2 {
	Cliques& target;

	void clique(auto const& clique, Graph const&) const {
		for (auto& t = target.emplace_back(); Graph::vertex_descriptor v : clique)
			t.push_back(v);
	}
};



// Add vertex or return existing one
Vertex add_vertex_by_name(Graph& g, std::map<std::string, Vertex>& name_to_vertex, const std::string& name) {
	auto it = name_to_vertex.find(name);
	if (it == name_to_vertex.end()) {
		// Create new vertex with name property
		Vertex v = boost::add_vertex(g);
		boost::put(boost::vertex_name, g, v, name);
		name_to_vertex[name] = v;
		return v;
	}
	return it->second;
}

//int fact(int n) {
//	int res = 1;
//	for (int i = 1; i <= n; i++) {
//		res *= i;
//	}
//	return res;
//}
//
int binomial(int n, int r) {
	if (n < 3) return 0;
	if (r > n) return 0;
	if (r == 0 || r == n) return 1;

	int result = 1;
	for (int i = 0; i < r; ++i) {
		result *= (n - i);
		result /= (i + 1);
	}
	return result;
}
//
//int subgraphs_with_t(int n, int m) {
//	int total = binomial(n, 3);
//
//	int no_t = binomial(n - m, 3);
//
//	return total - no_t;
//}
//
//struct edge {
//	std::string a, b;
//};

//std::ostream& operator<<(std::ostream& os, const edge& e) {
//	return os << e.a << " " << e.b;
//}

int main() {

	Graph g;
	std::map<std::string, Vertex> name_to_vertex;

	aoc_utils::read_lines_mmap_apply<void>("input.txt", [&](const std::string& line) {
		auto match = ctre::match<R"(([a-z]+)-([a-z]+))">(line);
		if (match) {
			//edge{ match.get<1>().to_string(), match.get<2>().to_string() };
			Vertex v1 = add_vertex_by_name(g, name_to_vertex, match.get<1>().to_string());
			Vertex v2 = add_vertex_by_name(g, name_to_vertex, match.get<2>().to_string());
			boost::add_edge(v1, v2, g);
		}

		});

	std::cout << binomial(4, 3) - binomial(3, 3) << '\n';

	
	
	int p1 = 0;

	/*for (auto& e : edges) {
		std::cout << e << '\n';
		Vertex v1 = add_vertex_by_name(g, name_to_vertex, e.a);
		Vertex v2 = add_vertex_by_name(g, name_to_vertex, e.b);
		boost::add_edge(v1, v2, g);
	}*/

	/*std::cout << "Graph vertices and edges:\n";
	boost::print_graph(g, boost::get(boost::vertex_name, g));*/

	std::vector<Clique> cliques;
	bron_kerbosch_all_cliques(g, Collector{ cliques }, 3);

	/*for (const auto& c : cliques) {
		std::cout << "Clique: ";
		for (V v : c) {
			std::cout << boost::get(boost::vertex_name, g, v) << ' ';
		}
		std::cout << '\n';
	}*/

	// get size of all cliques
	/*for (const auto& c : cliques) {
		int num_elements = c.size();
		int tc = 0;
		for (V v : c) {
			auto ele = boost::get(boost::vertex_name, g, v);
			if (ele.front() == 't') {
				tc++;
			}
		}
		p1 += subgraphs_with_t(num_elements, tc);
	}*/

	std::set<std::set<std::string>> unique_t_cliques;

	for (const auto& c : cliques) {
		bool has_t = false;
		std::set<std::string> clique_members;

		// First pass: collect members and check for 't'
		for (V v : c) {
			auto ele = boost::get(boost::vertex_name, g, v);
			clique_members.insert(ele);
			if (ele.front() == 't') {
				has_t = true;
			}
		}

		// If clique has 't', add it to our set of unique cliques
		if (has_t) {
			unique_t_cliques.insert(clique_members);

			// Debug print
			/*std::cout << "Found clique: ";
			for (const auto& member : clique_members) {
				std::cout << member << ' ';
			}
			std::cout << '\n';*/
		}
	}

	p1 = unique_t_cliques.size();

	std::string p2 = "";


	std::vector<Clique> cliques2;
	bron_kerbosch_all_cliques(g, Collector2{ cliques2 }, 3);
	
	auto maxC = std::max_element(cliques2.begin(), cliques2.end(), [](const auto& a, const auto& b) {
		return a.size() < b.size();
		});

	//std::cout << "Max clique size: " << maxC->size() << '\n';

	std::vector<std::string> ele;
	for (const auto& c : cliques2) {
		if (c.size() == maxC->size()) {
			for (V v : c) {
				ele.push_back(boost::get(boost::vertex_name, g, v));
			}
		}
	}

	/*for (const auto& e : ele) {
		std::cout << e << ",";
	}
	std::cout << '\n';*/
	//std::lexicographical_compare(ele.begin(), ele.end(), ele.begin(), ele.end());

	std::sort(ele.begin(), ele.end());

	for (const auto& e : ele) {
		//std::cout << e << '\n';
		p2 += std::string(e) + ",";
	}

	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2.substr(0, p2.length() - 1) << '\n';

}

// High 2061