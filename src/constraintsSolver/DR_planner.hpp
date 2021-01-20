
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

struct network;
using network_ptr = std::shared_ptr<network>;

struct node {
	std::vector<edge_ptr> incid;
	edge_ptr prevEdge;
	int weight;
	int scan, label;
	int pathCap;
	int currFlow;
};

struct edge {
	node_ptr a, b;
	node_ptr prevNode;
	int flowa, flowb;
	int weight;
	int scan, label;
	int pathCap;
};


struct graph {
	std::vector<node_ptr> nodes;
	std::vector<edge_ptr> edges;
};

struct network {
	std::vector<node_ptr> N;
	std::vector<edge_ptr> M;

	void strip_labels();
	int labeled_nodes();
	int scanned_nodes();
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

network_ptr create_network(graph graph_entry);

void restore_flow(edge_ptr ed);
void augment(node_ptr nd, edge_ptr ed);
int sum_flow(network_ptr net, node_ptr nd);
bool distribute(network_ptr G, edge_ptr ed);
bool distribute(network_ptr G, edge_ptr ed, node_ptr added_node, int k);

#endif