
#ifndef DR_PLANNER_HPP_
#define DR_PLANNER_HPP_

// https://www.cs.purdue.edu/homes/cmh/distribution/papers/Constraints/CP97.pdf

#include <memory>
#include <vector>

struct vertex;
struct edge;
struct bipartite_graph;
using vertex_ptr = std::shared_ptr<vertex>;
using edge_ptr = std::shared_ptr<edge>;
using bipartite_graph_ptr = std::shared_ptr<bipartite_graph>;

struct vertex {
	vertex(int w);
	edge_ptr prevEdge;
	int weight;
	int init_capacity, capacity;
	int label;
	int scan;
	int name;
	bool exists;
	static int counter;
};

struct edge {
	edge(vertex_ptr a_, vertex_ptr b_, int w);
	vertex_ptr prevVert;
	vertex_ptr a, b;
	int weight;
	int init_capacity, capacity;
	int flow_a, flow_b;
	int label;
	int scan;
	int name;
	bool exists;
	static int counter;
};

class bipartite_graph {
public:
	std::vector<vertex_ptr> N;
	std::vector<edge_ptr> M;
public:
	bipartite_graph(std::vector<vertex_ptr> aN, std::vector<edge_ptr> aM);

	std::vector<vertex_ptr> minimal(int k = 0);
	bool dense(int k, std::vector<vertex_ptr>& outDense, vertex_ptr& lastAdded);
	int distribute(edge_ptr e);

	int max_matching();
	void minCut();
	bool has_validFlow();
	
	int sum_incidentCapacity(vertex_ptr v);
	void label_incidents(vertex_ptr v);
	bool has_labeled_unscanned();
	int density(bool labeled_only = false);
	int density_withRespectTo(vertex_ptr v);

	std::vector<vertex_ptr> labeled_vertices();
	bipartite_graph_ptr induced_subgraph(std::vector<vertex_ptr> V);

	void clear_marquers();
	void clear_path();

	static bool is_incident(std::vector<vertex_ptr> G, edge_ptr ed);
private:
	bool dense_k_positive(int k, std::vector<vertex_ptr>& outDense, vertex_ptr& lastAdded);
	bool dense_k_negative(int k, std::vector<vertex_ptr>& outDense, vertex_ptr& lastAdded);

	bool matching(edge_ptr e);
	void label_reachableFromVertex(vertex_ptr v);
	void label_reachableFromEdge(edge_ptr e);
};

#endif