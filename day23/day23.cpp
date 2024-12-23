#include "../utils/utils.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/bron_kerbosch_all_cliques.hpp>

// Graph with string vertex labels
typedef boost::property<boost::vertex_name_t, std::string> VertexProperty;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexProperty> Graph;

// Mapping for vertex descriptors by name
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

using V = boost::graph_traits<Graph>::vertex_descriptor;

using Clique = std::vector<V, std::allocator<V>>;
using Cliques = std::vector<Clique>;


template <typename T>
std::vector<std::vector<T>> combinations(const std::vector<T>& elements, int k) {
	std::vector<std::vector<T>> res;
	std::vector<int> indices(k);
	int n = elements.size();

	for (int i = 0; i < k; ++i) {
		indices[i] = i;
	}

	while (true) {
		std::vector<T> subset;
		for (int i : indices) {
			subset.push_back(elements[i]);
		}
		res.push_back(subset);

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

	return res;
}


struct Collector {
	Cliques& target;

	template <typename VertexVector>
	void clique(const VertexVector& clique, const Graph& g) const {
		if (clique.size() >= 3) {
			std::vector<V> clique_vec(clique.begin(), clique.end());
			auto sub_cliques = combinations(clique_vec, 3);
			target.insert(target.end(), sub_cliques.begin(), sub_cliques.end());
		}
	}
};

struct Collector2 {
	Cliques& target;

	template <typename VertexVector>
	void clique(const VertexVector& clique, Graph const&) const {
		for (auto& t = target.emplace_back(); Graph::vertex_descriptor v : clique) {
			t.push_back(v);
		}
	}
};

Vertex add_vertex(Graph& g, std::map<std::string, Vertex>& name_to_vertex, const std::string& name) {
	auto it = name_to_vertex.find(name);
	if (it == name_to_vertex.end()) {
		Vertex v = boost::add_vertex(g);
		boost::put(boost::vertex_name, g, v, name);
		name_to_vertex[name] = v;
		return v;
	}
	return it->second;
}

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

int main() {

	INITIALIZE_AOC_TIMERS();

	default_timer.begin(0);

	Graph g;
	std::map<std::string, Vertex> name_to_vertex;

	aoc_utils::read_lines_mmap_apply<void>("input.txt", [&](const std::string& line) {
		auto match = ctre::match<R"(([a-z]+)-([a-z]+))">(line);
		if (match) {
			Vertex v1 = add_vertex(g, name_to_vertex, match.get<1>().to_string());
			Vertex v2 = add_vertex(g, name_to_vertex, match.get<2>().to_string());
			boost::add_edge(v1, v2, g);
		}

		});

	default_timer.end(0);

	default_timer.begin(1);

	int p1 = 0;

	//boost::print_graph(g, boost::get(boost::vertex_name, g));

	std::vector<Clique> cliques;
	bron_kerbosch_all_cliques(g, Collector{ cliques }, 3);

	std::set<std::set<std::string>> unique_t_cliques;

	for (const auto& c : cliques) {
		bool has_t = false;
		std::set<std::string> clique_members;

		for (V v : c) {
			auto ele = boost::get(boost::vertex_name, g, v);
			clique_members.insert(ele);
			if (ele.front() == 't') {
				has_t = true;
			}
		}

		if (has_t) {
			unique_t_cliques.insert(clique_members);
		}
	}

	p1 = unique_t_cliques.size();

	default_timer.end(1);

	default_timer.begin(2);

	std::string p2 = "";


	std::vector<Clique> cliques2;
	bron_kerbosch_all_cliques(g, Collector2{ cliques2 }, 3);
	
	auto maxC = std::max_element(cliques2.begin(), cliques2.end(), [](const auto& a, const auto& b) {
		return a.size() < b.size();
		});


	std::vector<std::string> ele;
	for (const auto& c : cliques2) {
		if (c.size() == maxC->size()) {
			for (V v : c) {
				ele.push_back(boost::get(boost::vertex_name, g, v));
			}
		}
	}

	std::sort(ele.begin(), ele.end());

	for (const auto& e : ele) {
		p2 += std::string(e) + ",";
	}

	default_timer.end(2);

	std::cout << "Part 1: " << p1 << '\n' << "Part 2: " << p2.substr(0, p2.length() - 1) << '\n';

	default_timer.display_all();
}

//Part 1: 1269
//Part 2 : ad, jw, kt, kz, mt, nc, nr, sb, so, tg, vs, wh, yh
//============================== Timer Details ==============================
//Timer ID : 0
//Label : Input
//Description : Read input from file and parse
//Elapsed Time : 1471.2 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 10900.6 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 4077.9 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 34
//Ticks : 346182
//TotalDays : 4.00673611111111E-07
//TotalHours : 9.61616666666667E-06
//TotalMinutes : 0.00057697
//TotalSeconds : 0.0346182
//TotalMilliseconds : 34.6182