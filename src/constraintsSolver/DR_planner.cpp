
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

network_ptr create_network(graph graph_entry)
{
	return network_ptr(new network({graph_entry.nodes, graph_entry.edges}));
}

void network::strip_labels()
{
	for(int i = 0; i < N.size(); ++i) {
		N[i]->label = 0;
		N[i]->scan = 0;
	}
	for(int i = 0; i < M.size(); ++i) {
		M[i]->label = 0;
		M[i]->scan = 0;
	}
}
int network::labeled_nodes()
{
	int running_sum = 0;
	for(int i = 0; i < N.size(); ++i) {
		if(N[i]->label)
			running_sum++;
	}
	for(int i = 0; i < M.size(); ++i) {
		if(M[i]->label)
			running_sum++;
	}
	return running_sum;
}
int network::scanned_nodes()
{
	int running_sum = 0;
	for(int i = 0; i < N.size(); ++i) {
		if(N[i]->scan)
			running_sum++;
	}
	for(int i = 0; i < M.size(); ++i) {
		if(M[i]->scan)
			running_sum++;
	}
	return running_sum;
}

void augment(node_ptr nd, edge_ptr ed)
{
	int w = std::min(nd->pathCap, nd->weight - nd->currFlow);
	ed->pathCap -= w;

	node_ptr n = nd;
	edge_ptr e = n->prevEdge;

	while(e) {
		if(e->a == n) {
			e->flowa += w;
			n->currFlow += w;
		}
		if(e->b == n) {
			e->flowb += w;
			n->currFlow += w;
		}

		if(e->prevNode) {
			n = e->prevNode;

			if(e->a == n) {
				e->flowa -= w;
				n->currFlow -= w;
			}
			if(e->b == n) {
				e->flowb -= w;
				n->currFlow -= w;
			}

			e = n->prevEdge;
		} else {
			e = nullptr;
		}
	}
}

void restore_flow(edge_ptr ed)
{
	ed->a->currFlow -= ed->flowa;
	ed->b->currFlow -= ed->flowb;

	ed->flowa = 0;
	ed->flowb = 0;	
}

int sum_flow(network_ptr net, node_ptr nd)
{
	int running_sum = 0;
	for(int i = 0; i < net->M.size(); ++i) {
		edge_ptr e = net->M[i];
		if(e->a == nd) {
			running_sum += e->flowa;
		} else if(e->b == nd) {
			running_sum += e->flowb;
		}
	}
	return running_sum;
}

bool distribute(network_ptr G, edge_ptr ed)
{
	int n_labeled_unscanned = 1;

	G->strip_labels();

	int capNode = 0;
	node_ptr nd;
	ed->label = 1;
	ed->pathCap = ed->weight;
	ed->prevNode = nullptr;

	while((ed->flowa + ed->flowb) < ed->weight || n_labeled_unscanned > 0) {
		n_labeled_unscanned = 0;

		for(int i = 0; i < G->M.size(); ++i) {
			edge_ptr e = G->M[i];
			if(!e->label || e->scan != 0) // For every labeled but non-scanned edge
				continue;
			
			// An edge only has two endpoints, label unlabeled neighbors(end points)
			if(!e->a->label) {
				e->a->label = 1;
				e->a->pathCap = e->pathCap;
				e->a->prevEdge = e;
				n_labeled_unscanned++;
			}
			if(!e->b->label) {
				e->b->label = 1;
				e->b->pathCap = e->pathCap;
				e->b->prevEdge = e;
				n_labeled_unscanned++;
			}
			e->scan = 1;
		}

		for(int i = 0; i < G->N.size(); ++i) {
			node_ptr n = G->N[i];
			if(!n->label || n->scan != 0) // For every labeled but non-scanned node
				continue;
			int attempt_path_cap = std::min(n->weight-sum_flow(G, n), n->pathCap);
			if(attempt_path_cap > capNode) {
				nd = n;
				capNode = attempt_path_cap;

				if(ed->pathCap > 0)
					ed->label = 1;
			} else {
				for(int j = 0; j < G->M.size(); ++j) { // for every non labeled edge incident to n (TODO: use n->incid instead??)
					edge_ptr e = G->M[i];
					if(!e->label && e->a == n) {
						e->label = 1;
					}
					if(!e->label && e->b == n) {
						e->label = 1;
					}
				}
			}
			n->scan = 1;
		}

		if(nd) {
			augment(nd, ed);
			G->strip_labels();
			nd = nullptr;
			capNode = 0;
			ed->label = 1;
			ed->pathCap = ed->weight - (ed->flowa + ed->flowb);
		}
	}

	return ed->pathCap > 0;
}

bool distribute(network_ptr G, edge_ptr ed, node_ptr added_node, int k)
{
	if(!distribute(G, ed))
		return false;
	
	ed->pathCap -= (added_node->weight + k);
	if(!distribute(G, ed))
		return false;
	
	restore_flow(ed);

	return true;
}