
#ifndef DR_PLANNER_HPP_
#define DR_PLANNER_HPP_

// https://www.cise.ufl.edu/~sitharam/pdfs/drone-final.pdf
// https://www.cs.purdue.edu/homes/cmh/distribution/PubsCons.html
// https://www.cs.purdue.edu/homes/cmh/distribution/papers/Constraints/JSC01b.pdf

#include <memory>
#include <vector>
#include <iostream>

class graph;
class cluster;
using graph_ptr = std::shared_ptr<graph>;
using cluster_ptr = std::shared_ptr<cluster>;

struct node;
struct edge;
using node_ptr = std::shared_ptr<node>;
using edge_ptr = std::shared_ptr<edge>;

struct node {
	std::vector<edge_ptr> incidents;
	int weight;
};

struct edge {
	node_ptr a, b;
	bool directed;
	int weight;
	int junk;
};

struct graph {
	std::vector<node_ptr> nodes;
	std::vector<edge_ptr> edges;
};

struct cluster {
	int weight;
	std::vector<node_ptr> frontier;
};

std::vector<cluster_ptr> detect_clusters(graph_ptr constraint_graph);
graph_ptr skeletonize(graph_ptr constraint_graph, std::vector<cluster_ptr> clusters);
void place_clusters(graph_ptr constraint_graph, graph_ptr next_constraint_graph);
void place_nodes_in_plane(graph_ptr constraint_graph);
void solve(graph_ptr constraint_graph);

node_ptr create_network(graph graph_entry);

void print_node(node_ptr nd, int stage = 0);
void print_edge(edge_ptr nd, int stage = 0);

#endif