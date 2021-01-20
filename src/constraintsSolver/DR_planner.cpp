
#include "DR_planner.hpp"

std::vector<cluster_ptr> detect_clusters(graph_ptr constraint_graph)
{
	return {};
}
graph_ptr skeletonize(graph_ptr constraint_graph, std::vector<cluster_ptr> clusters)
{
	return nullptr;
}
void place_clusters(graph_ptr constraint_graph, graph_ptr next_constraint_graph)
{

}
void place_nodes_in_plane(graph_ptr constraint_graph)
{

}
void solve(graph_ptr constraint_graph)
{
	std::vector<cluster_ptr> clusters = detect_clusters(constraint_graph);

	if(clusters.size() > 1) {
		graph_ptr next_constraint_graph = skeletonize(constraint_graph, clusters);
		solve(next_constraint_graph);
		place_clusters(constraint_graph, next_constraint_graph);
	} else {
		place_nodes_in_plane(constraint_graph);
	}
}

node_ptr create_network(graph graph_entry)
{
	node_ptr source(new node{{}, 0});
	node_ptr sink(new node{{}, 0});

	for(int i = 0; i < graph_entry.edges.size(); ++i) {
		node_ptr edge_node(new node({{}, 7}));
		edge_ptr ed(new edge({source, edge_node, true, graph_entry.edges[i]->weight}));
		graph_entry.edges[i]->junk = i;
		edge_node->incidents.push_back(ed);
		source->incidents.push_back(ed);
	}

	for(int i = 0; i < graph_entry.nodes.size(); ++i) {
		node_ptr node_node(new node({{}, 5}));
		edge_ptr ed(new edge({node_node, sink, true, graph_entry.nodes[i]->weight}));

		for(int j = 0; j < graph_entry.nodes[i]->incidents.size(); ++j) {
			edge_ptr edge_node_to_node_node(new edge({source->incidents[graph_entry.nodes[i]->incidents[j]->junk]->b, node_node, true, 0, 0}));
			source->incidents[graph_entry.nodes[i]->incidents[j]->junk]->b->incidents.push_back(edge_node_to_node_node);
			node_node->incidents.push_back(edge_node_to_node_node);
		}
		
		node_node->incidents.push_back(ed);
		sink->incidents.push_back(ed);
	}
	return source;
}

void print_node(node_ptr nd, int stage)
{
	std::string offset = "";
	for(int i = 0; i < stage; ++i) {
		offset += '\t';
	}
	std::cout<<offset<<nd->weight<<"\n";
	for(int i = 0; i < nd->incidents.size(); ++i) {
		if(!nd->incidents[i]->directed || nd->incidents[i]->a == nd)
			print_edge(nd->incidents[i], stage+1);
	}
}
void print_edge(edge_ptr ed, int stage)
{
	std::string offset = "";
	for(int i = 0; i < stage; ++i) {
		offset += '\t';
	}
	if(ed->directed) {
		std::cout<<offset<<"["<<ed->weight<<"]"<<"=>\n";
	} else {
		std::cout<<offset<<"["<<ed->weight<<"]"<<"==\n";
	}
	if(!ed->directed)
		print_node(ed->a, stage+1);
	print_node(ed->b, stage+1);
}