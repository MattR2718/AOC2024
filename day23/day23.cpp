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


// Work out all combinations of k elements from a vector
// Returns elements sorted
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

struct Collector_Maxmial {
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
	bron_kerbosch_all_cliques(g, Collector_Maxmial{ cliques }, 3);

	std::set<Clique> unique_t_cliques;

	for (const auto& c : cliques) {

		auto clis = combinations(c, 3);
		for (const auto& cli : clis) {
			bool has_t = false;
			for (V v : cli) {
				auto ele = boost::get(boost::vertex_name, g, v);
				if (ele.front() == 't') {
					has_t = true;
				}
			}
			if (has_t) {
				unique_t_cliques.insert(cli);
			}
		}
	}

	p1 = unique_t_cliques.size();

	default_timer.end(1);

	default_timer.begin(2);

	std::string p2 = "";
	
	auto maxC = std::max_element(cliques.begin(), cliques.end(), [](const auto& a, const auto& b) {
		return a.size() < b.size();
		});

	std::vector<std::string> ele;
	for (V v : *maxC) {
		ele.push_back(boost::get(boost::vertex_name, g, v));
	}

	std::sort(ele.begin(), ele.end());

	for (const auto& e : ele) {
		p2 += e + ",";
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
//Elapsed Time : 1357.5 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 1
//Label : Part 1
//Description : Compute part 1
//Elapsed Time : 6419.4 microseconds
//========================================================================== =
//============================== Timer Details ==============================
//Timer ID : 2
//Label : Part 2
//Description : Compute part 2
//Elapsed Time : 2.6 microseconds
//========================================================================== =
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 25
//Ticks : 259856
//TotalDays : 3.00759259259259E-07
//TotalHours : 7.21822222222222E-06
//TotalMinutes : 0.000433093333333333
//TotalSeconds : 0.0259856
//TotalMilliseconds : 25.9856