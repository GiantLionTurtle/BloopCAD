
#include "DR_planner.hpp"

#include <utils/errorLogger.hpp>

#include <cmath>
#include <limits>

int vertex::counter = 0;
int edge::counter = 0;

vertex::vertex(int w):
	prevEdge(nullptr),
	weight(w),
	init_capacity(0),
	capacity(0),
	label(0),
	scan(0),
	exists(true),
	name(++counter)
{

}

edge::edge(vertex_ptr a_, vertex_ptr b_, int w):
	prevVert(nullptr),
	a(a_),
	b(b_),
	weight(w),
	init_capacity(0),
	capacity(0),
	flow_a(0),
	flow_b(0),
	label(0),
	scan(0),
	exists(true),
	name(++counter)
{

}

bipartite_graph::bipartite_graph(std::vector<vertex_ptr> aN, std::vector<edge_ptr> aM):
	N(aN),
	M(aM)
{

}

int bipartite_graph::max_matching()
{
	int out = 0;
	for(int i = 0; i < M.size(); ++i) {
		edge_ptr e = M[i];

		clear_marquers();

		out += matching(e);
	}
	return out;
}

void bipartite_graph::clear_marquers()
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
void bipartite_graph::clear_path()
{
	for(int i = 0; i < N.size(); ++i) {
		N[i]->prevEdge = nullptr;
	}
	for(int i = 0; i < M.size(); ++i) {
		M[i]->prevVert = nullptr;
	}
}

void bipartite_graph::minCut()
{
	while(has_validFlow()) {
		int path_flow = std::numeric_limits<int>::max();
		for(int i = 0; i < N.size(); ++i) {
			vertex_ptr v = N[i];
			if(v->label && v->prevEdge) {
				path_flow = std::min(std::min(path_flow, v->capacity), v->prevEdge->capacity);
			}
		}
		for(int i = 0; i < N.size(); ++i) {
			vertex_ptr v = N[i];
			if(v->label && v->prevEdge) {
				if(v->prevEdge->a == v) {
					v->prevEdge->flow_a += path_flow;
				} else if(v->prevEdge->b == v) {
					v->prevEdge->flow_b += path_flow;
				} else {
					LOG_WARNING("What's going on here..?");
				}
			}
		}
	}
}

bool bipartite_graph::has_validFlow()
{
	clear_marquers();
	clear_path();

	for(int i = 0; i < M.size(); ++i) {
		edge_ptr e = M[i];
		if(!e->label && (e->flow_a + e->flow_b) < e->capacity) {
			e->label = true;

			for(int j = 0; j < N.size(); ++j) {
				vertex_ptr v = N[j];
				// if(!v->label && v->capacity > 0) {
				if((e->a == v || e->b == v) && !v->label && v->capacity > 0) {	
					v->label = true;
					v->prevEdge = e;
					return true;
				}
			}
		}
	}

	return false;
}

int bipartite_graph::sum_capIncid(vertex_ptr v)
{
	if(!v->exists)	
		return 0;
	int running_sum = 0;
	for(int i = 0; i < M.size(); ++i) {
		edge_ptr e = M[i];
		if(!e->exists)
			continue;
		if(e->a == v) {
			running_sum += e->flow_a;
		} else if(e->b == v) {
			running_sum += e->flow_b;
		}
	}
	return running_sum;
}

void bipartite_graph::label_incid(vertex_ptr v)
{
	if(!v->exists)
		return;
	int running_sum = 0;
	for(int i = 0; i < M.size(); ++i) {
		edge_ptr e = M[i];
		if(!e->exists)
			continue;
		if(e->a == v && e->flow_a > 0) {
			e->label = true;
		} else if(e->b == v && e->flow_b > 0) {
			e->label = true;
		}
	}
}

bool bipartite_graph::is_incident(std::vector<vertex_ptr> G, edge_ptr ed)
{
	if(!ed->exists)
		return false;
	for(int i = 0; i < G.size(); ++i) {
		vertex_ptr v = G[i];
		if(!v->exists)
			continue;
		if(ed->a == v || ed->b == v) 
			return true;
	}
	return false;
}

bool bipartite_graph::has_labeled_unscanned()
{
	for(int i = 0; i < N.size(); ++i) {
		vertex_ptr v = N[i];
		if(v->label && !v->scan && v->exists)
			return true;
	}
	for(int i = 0; i < M.size(); ++i) {
		edge_ptr e = M[i];
		if(e->label && !e->scan && e->exists)
			return true;
	}
	return false;
}

int bipartite_graph::distribute(edge_ptr ed)
{
	if(!ed->exists) {
		LOG_WARNING("What's going on here..");
		return density();
	}
	clear_path();
	clear_marquers();

	vertex_ptr vert = nullptr;
	int capvert = 0;
	ed->label = true;
	ed->capacity = ed->weight;
	while(ed->weight > (ed->flow_a + ed->flow_b) || has_labeled_unscanned()) {
		for(int i = 0; i < M.size(); ++i) {
			edge_ptr e = M[i];
			if(!e->exists)
				continue;
			if(e->label && !e->scan) {
				e->a->label = true;
				// e->a->scan = false;
				e->a->prevEdge = e;
				e->a->capacity = e->capacity;
				e->b->label = true;
				// e->b->scan = false; 
				e->b->prevEdge = e;
				e->b->capacity = e->capacity;
				e->scan = true;
			}
		}
		for(int i = 0; i < N.size(); ++i) {
			vertex_ptr v = N[i];
			if(!v->exists)
				continue;
			if(v->label && !v->scan) {
				int capvert_attempt = std::min(v->weight - sum_capIncid(v), v->capacity);
				// std::cout<<"attempt: "<<capvert_attempt<<"\n";
				if(capvert_attempt > capvert) {
					vert = v;
					capvert = capvert_attempt;
				} else {
					label_incid(v);
				}
				v->scan = true;
			}
		}
		if(vert) {
			if(vert->prevEdge) {
				int mod = 0;
				if(vert->prevEdge->a == vert) {
					mod = std::min(vert->weight - vert->prevEdge->flow_a, vert->prevEdge->capacity);
					vert->prevEdge->flow_a += mod;
					// std::cout<<__LINE__<<": flow_a = "<<vert->prevEdge->flow_b<<std::endl;
				} else if(vert->prevEdge->b == vert) {
					mod = std::min(vert->weight - vert->prevEdge->flow_b, vert->prevEdge->capacity);
					vert->prevEdge->flow_b += mod;
					// std::cout<<__LINE__<<": flow_b = "<<vert->prevEdge->flow_b<<std::endl;
				} else {
					LOG_WARNING("Peculiar...");
				}
				vert->capacity -= mod;
				// std::cout<<__LINE__<<": mod, capacity = "<<mod<<",  "<<vert->capacity<<std::endl;
			}
			clear_marquers();
			vert = nullptr;
			capvert = 0;
			ed->label = true;
			ed->capacity = ed->weight - (ed->flow_a + ed->flow_b);
		}		
	}

	return density(true);
}

int bipartite_graph::density(bool labeled_only)
{
	int distribute_balance = 0;
	for(int i = 0; i < N.size(); ++i) {
		vertex_ptr v = N[i];
		if(v->exists && (!labeled_only || v->label))
			distribute_balance -= v->weight;
	}
	for(int i = 0; i < M.size(); ++i) {
		edge_ptr e = M[i];
		if(e->exists && (!labeled_only || e->label))
			distribute_balance += (e->flow_a + e->flow_b);
	}
	return distribute_balance;
}

int tmp_density(bipartite_graph_ptr G, vertex_ptr v)
{
	if(!v->exists) {
		LOG_WARNING("You ok bro?");
	}
	int den = 0;
	for(int i = 0; i < G->M.size(); ++i) {
		edge_ptr e = G->M[i];
		if(!e->exists)
			continue;
		if(e->a == v || e->b == v) 
			den += -e->weight + e->flow_a + e->flow_b;
	}
	return den;
}

std::vector<vertex_ptr> bipartite_graph::minimal(int k)
{
	std::vector<vertex_ptr> A_vertices;
	vertex_ptr lastAdded;
	if(!dense(k, A_vertices, lastAdded)) 
		return {};
	
	bipartite_graph_ptr A = induced_subgraph(A_vertices);
	std::vector<vertex_ptr> B = { lastAdded };

	for(int i = 0; i < A_vertices.size(); ++i) {
		if(A_vertices[i] == lastAdded) {
			std::cout<<"last added in A!\n";
		}
	}
	for(int i = 0; i < N.size(); ++i) {
		if(N[i] == lastAdded) {
			std::cout<<"last added in N!\n";
		}
	}
	int density_A;
	int density_B;
	while((density_B = induced_subgraph(B)->density()) <= 0 && (density_A = tmp_density(A, lastAdded) > (k+1))) {
		std::cout<<__LINE__<<": d = "<<density_A<<",  "<<density_B<<"\n";
		vertex_ptr v = nullptr;
		bool v_in_B = false;
		edge_ptr e_ = nullptr;
		for(int i = 0; i < M.size() && !e_; ++i) {
			edge_ptr tmp_e = M[i];
			if(tmp_e->a == lastAdded) {
				v_in_B = false;
				for(int j = 0; j < B.size(); ++j) {
					if(tmp_e->b == B[j]) {
						v_in_B = true;
					}
				}
				if(!v_in_B) {
					BLOOP_MARKER;
					v = tmp_e->b;
					e_ = tmp_e;
				}
			} else if(tmp_e->b == lastAdded) {
				v_in_B = false;
				for(int j = 0; j < B.size(); ++j) {
					if(tmp_e->a == B[j]) {
						v_in_B = true;
					}
				}
				if(!v_in_B) {
					BLOOP_MARKER;
					v = tmp_e->a;
					e_ = tmp_e;
				}
			}
		}
		if(!e_) {
			LOG_ERROR("This evolved quickly into a big freakin problem.");
		}
		edge_ptr e_that_exists = nullptr;
		for(int i = 0; i < M.size(); ++i) {
			edge_ptr e = M[i];
			if(e->a == v || e->b == v) {
				e->exists = false;
			} else if(!e_that_exists && e->a == lastAdded || e->b == lastAdded) {
				e_that_exists = e;
			}
		}
		if(!e_that_exists) {
			LOG_ERROR("Fuck.");
		}
		v->exists = false;
		std::cout<<"v, lastAdded: "<<v->name<<",  "<<lastAdded->name<<"\n";
		std::cout<<"E_: "<<e_->name<<"\n";
		std::cout<<"Distributing e: "<<e_that_exists->name<<"\n";
		int c = tmp_density(A, lastAdded) - e_->weight + e_->flow_a + e_->flow_b;
		std::cout<<__LINE__<<" c = "<<c<<"\n";
		int init_cap = v->capacity;
		v->capacity = 0;
		A->distribute(e_that_exists);
		if(tmp_density(A, lastAdded) > c) {
			std::cout<<__LINE__<<": d = "<<tmp_density(A, lastAdded)<<",  "<<induced_subgraph(B)->density()<<"\n";
			std::vector<vertex_ptr> out = labeled_vertices();
			std::cout<<"n edges: "<<induced_subgraph(out)->M.size()<<"\n";
			return out;
		}
		std::cout<<__LINE__<<": d = "<<tmp_density(A, lastAdded)<<",  "<<induced_subgraph(B)->density()<<"\n";

		v->capacity = init_cap;
		for(int i = 0; i < M.size(); ++i) {
			edge_ptr e = M[i];
			e->exists = true;
		}
		v->exists = true;
		B.push_back(v);
	}

	std::cout<<__LINE__<<": d = "<<density_A<<",  "<<density_B<<"\n";
	if(induced_subgraph(B)->density() > 0) {
		std::cout<<"Return B!\n";
		return B;
	}
	std::cout<<"Return A!\n";
	return A->N;
}

bipartite_graph_ptr bipartite_graph::induced_subgraph(std::vector<vertex_ptr> V)
{
	clear_marquers();
	for(int i = 0; i < V.size(); ++i) {
		V[i]->label = true;
	}

	std::vector<edge_ptr> edges;
	for(int i = 0; i < M.size(); ++i) {
		edge_ptr e = M[i];
		if(!e->exists)
			continue;
		if(e->a->label && e->b->label)
			edges.push_back(e);
	}
	return std::make_shared<bipartite_graph>(V, edges);
}

bool bipartite_graph::dense(int k, std::vector<vertex_ptr>& outDense, vertex_ptr& lastAdded)
{
	if(k >= 0) {
		return dense_k_positive(k, outDense, lastAdded);
	} else {
		return dense_k_negative(k, outDense, lastAdded);
	}
}

std::vector<vertex_ptr> bipartite_graph::labeled_vertices()
{
	std::vector<vertex_ptr> out;
	for(int i = 0; i < N.size(); ++i) {
		vertex_ptr v = N[i];
		if(v->label)
			out.push_back(v);
	}
	return out;
}

bool bipartite_graph::matching(edge_ptr e)
{
	for(int i = 0; i < N.size(); ++i) {
		vertex_ptr v = N[i];
		if((e->a == v || e->b == v) && !v->label) {
			v->label = true;

			if(!v->prevEdge || matching(v->prevEdge)) {
				v->prevEdge = e;
				return true;
			}
		}
	}
	return false;
}

void bipartite_graph::label_reachableFromVertex(vertex_ptr v)
{
	v->label = true;
	for(int i = 0; i < M.size(); ++i) {
		edge_ptr e = M[i];
		if(!e->label && (e->a == v || e->b == v)) {
			label_reachableFromEdge(e);
		}
	}
}
void bipartite_graph::label_reachableFromEdge(edge_ptr e)
{
	e->label = true;
	for(int i = 0; i < N.size(); ++i) {
		vertex_ptr v = N[i];
		if(!v->label && (e->a == v || e->b == v)) {
			label_reachableFromVertex(v);
		}
	}
}

bool bipartite_graph::dense_k_positive(int k, std::vector<vertex_ptr>& outDense, vertex_ptr& lastAdded)
{
	std::vector<vertex_ptr> G_;
	for(int i = 0; i < N.size(); ++i) {
		vertex_ptr v = N[i];
		for(int j = 0; j < M.size(); ++j) {
			edge_ptr e = M[i];
			if(!is_incident(G_, e) || (e->a == v || e->b == v))
				continue;
			if(distribute(e) > k) {
				outDense = labeled_vertices();
				lastAdded = G_.back();
				return true;
			}
		}
		G_.push_back(v);
	}
	return false;
}
bool bipartite_graph::dense_k_negative(int k, std::vector<vertex_ptr>& outDense, vertex_ptr& lastAdded)
{
	std::vector<vertex_ptr> G_;
	for(int i = 0; i < N.size(); ++i) {
		vertex_ptr v = N[i];
		for(int j = 0; j < M.size(); ++j) {
			edge_ptr e = M[j];
			if(!is_incident(G_, e) || !(e->a == v || e->b == v))
				continue;
			if(v->weight + k >= 0) {
				int init_cap = v->capacity;
				v->capacity = v->weight + k;
				if(distribute(e) > 0) {
					outDense = labeled_vertices();
					lastAdded = G_.back();
					return true;
				}
				v->capacity = init_cap;
			} else {
				// int init_v_cap = v->capacity;
				v->capacity = 0;
				if(distribute(e) > 0) {
					outDense = labeled_vertices();
					lastAdded = G_.back();
					return true;
				}
				int init_cap = e->capacity;
				e->capacity -= (v->weight + k);
				if(distribute(e) > 0) {
					outDense = labeled_vertices();
					lastAdded = G_.back();
					return true;
				}
				e->capacity = init_cap;
				// v->capacity = init_v_cap;
			}
		}
		G_.push_back(v);
	}
	return false;
}

		// for(int i = 0; i < A->N.size(); ++i) {
		// 	v = A->N[i];
		// 	if(v == lastAdded)
		// 		continue;
		// 	found_v = false;
		// 	for(int j = 0; j < B.size() && !found_v; ++j) {
		// 		if(v == B[i]) {
		// 			found_v = true;
		// 			break;
		// 		}
		// 	}
		// 	if(!found_v)
		// 		break;
		// }
		// if(found_v) {
		// 	LOG_WARNING("Whoopsie..");
		// }