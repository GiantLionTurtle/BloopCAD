
#include "DR_planner.hpp"

#include <utils/errorLogger.hpp>

#include <cmath>
#include <limits>
#include <algorithm>

int cluster::counter = 0;
int edge::counter = 0;

cluster::cluster(int w):
	mSubClusters(0),
	mSubClusters_edges(0),
	mIncidentEdges(0),
	mPrevEdge(nullptr),
	mWeight(w),
	mCapacity(0),
	mDensity(w),
	mLabel(0),
	mScan(0),
	mExists(true),
	mAvailable_for_dense(true),
	mName(++counter)
{

}
cluster::cluster(std::vector<cluster_ptr> clusters, cluster* base):
	mSubClusters(clusters),
	mSubClusters_edges(0),
	mIncidentEdges(0),
	mPrevEdge(nullptr),
	mWeight(0),
	mCapacity(0),
	mDensity(0),
	mLabel(0),
	mScan(0),
	mExists(true),
	mAvailable_for_dense(true),
	mName(++counter)
{
	label_allNodes_recursive(2);

	for(cluster_ptr clus : mSubClusters) {
		for(edge_ptr edg : clus->incidentEdges()) {
			if(edg->a()->label() == 2 && edg->b()->label() == 2 && edg->label() != 2) {
				edg->set_endPointIncidence();
				edg->set_label(2);
				mSubClusters_edges.push_back(edg);
			}
		}
	}

	if(base) {
		for(edge_ptr edg : base->mSubClusters_edges) {
			if((edg->a()->label() == 2 || edg->b()->label() == 2) && edg->label() != 2) {
				edg->set_endPointIncidence();
				edg->set_label(2);
				add_incidentEdge(edg);
			}
		}
	}

	label_allNodes_recursive(0);
	compute_density();
}

cluster::cluster(std::vector<cluster_ptr> clusters, std::vector<edge_ptr> edges, cluster* base):
	mSubClusters(clusters),
	mSubClusters_edges(0),
	mIncidentEdges(0),
	mPrevEdge(nullptr),
	mWeight(0),
	mCapacity(0),
	mDensity(0),
	mLabel(0),
	mScan(0),
	mExists(true),
	mAvailable_for_dense(true),
	mName(++counter)
{
	label_allNodes_recursive(2);

	for(edge_ptr edg : edges) {
		if(edg->a()->label() == 2 && edg->b()->label() == 2 && edg->label() != 2) {
			edg->set_endPointIncidence();
			edg->set_label(2);
			mSubClusters_edges.push_back(edg);
		}
	}

	if(base) {
		for(edge_ptr edg : base->mSubClusters_edges) {
			if((edg->a()->label() == 2 || edg->b()->label() == 2) && edg->label() != 2) {
				edg->set_endPointIncidence();
				edg->set_label(2);
				add_incidentEdge(edg);
			}
		}
	}

	label_allNodes_recursive(0);
	compute_density();
}

std::vector<cluster_ptr> cluster::clusters(int k)
{
	std::vector<cluster_ptr> out;
	cluster_ptr subCluster = nullptr;
	while((subCluster = find_cluster(out, k))) {
		out.push_back(subCluster);
	}
	return out;
}

cluster_ptr cluster::find_cluster(std::vector<cluster_ptr>& root_mask, int k)
{
	for(cluster_ptr clust : root_mask) {
		clust->set_available_for_dense_recursive(false);
	}

	for(cluster_ptr clust : mSubClusters) {
		if(clust->available_for_dense()) {
			return extend(minimal(-4));
		}
	}

	for(cluster_ptr clust : root_mask) {
		clust->set_available_for_dense_recursive(true);
	}
	return nullptr;
}

cluster_ptr cluster::simplify(int k)
{
	cluster_ptr simplified = reduce(k);
	cluster_ptr tmp_simplified = nullptr;
	std::cout<<"Simplify!\n";
	while(simplified->mSubClusters.size() > 1) {
		tmp_simplified = simplified->reduce(k);
		if(!tmp_simplified) {
			LOG_WARNING("Could not simplify to single node.");
			break;
		}
		simplified = tmp_simplified;

		std::cout<<"Simplify!\n";
	}

	return simplified;
}
cluster_ptr cluster::reduce(int k) 
{
	cluster_ptr subgraph = extend(minimal(k));
	if(!subgraph)
		return nullptr;
	subgraph->set_weight(-(k+1));
	reset_childrenMarquers();
	subgraph->set_childrenLabels(2);
	std::vector<cluster_ptr> reduced_clusters(mSubClusters.size() - subgraph->mSubClusters.size() + 1);
	int ind = 0;
	for(cluster_ptr clust : mSubClusters) {
		if(clust->label() != 2) {
			reduced_clusters[ind++] = clust;
		}
	}
	reduced_clusters[ind++] = subgraph;
	subgraph->reset_childrenMarquers();
	// subgraph->reroute_incidentEdges();
	return std::make_shared<cluster>(reduced_clusters, this);
}
cluster_ptr cluster::extend(cluster_ptr min)
{
	if(!min)
		return nullptr;
	cluster_ptr newGraph(new cluster(min->mSubClusters, min->mSubClusters_edges, this));
	int init_size = 0;
	reset_childrenMarquers();
	while(newGraph->mSubClusters.size() > init_size) {
		init_size = newGraph->mSubClusters.size();
		newGraph->set_childrenLabels(1);
		newGraph->label_incidentEdges();

		for(cluster_ptr clust : mSubClusters) {
			if(!clust->label() && clust->sum_incidentEdges_weight(true) >= clust->weight()) {
				newGraph->add_cluster(clust);
			}
		}
	}
	return newGraph;
}
cluster_ptr cluster::minimal(int k)
{
	std::vector<cluster_ptr> dense_clusters;
	cluster_ptr lastAdded;
	if(!dense(k, dense_clusters, lastAdded)) 
		return nullptr;
	
	cluster_ptr A(new cluster(dense_clusters));
	cluster_ptr B(new cluster({ lastAdded }));

	int density_A;
	int density_B;
	while((density_B = B->density() <= 0 && (density_A = A->density_around(lastAdded)) > (/*k+*/1))) {
		cluster_ptr AExclusive = nullptr;
		edge_ptr last_to_Aexclusive = nullptr;
		for(edge_ptr e : mSubClusters_edges) {
			bool cluster_in_B = false;
			if(e->a() == lastAdded) {
				for(cluster_ptr B_clust : B->mSubClusters) {
					if(e->b() == B_clust) {
						cluster_in_B = true;
					}
				}
				if(!cluster_in_B) {
					AExclusive = e->b();
					last_to_Aexclusive = e;
					break;
				}
			} else if(e->b() == lastAdded) {
				for(cluster_ptr B_clust : B->mSubClusters) {
					if(e->a() == B_clust) {
						cluster_in_B = true;
					}
				}
				if(!cluster_in_B) {
					AExclusive = e->a();
					last_to_Aexclusive = e;
					break;
				}
			}
		}
		if(!last_to_Aexclusive) {
			LOG_ERROR("Fatal error, terminating.");
		}
		edge_ptr not_in_AExclusive_clust = nullptr;
		for(edge_ptr e : mSubClusters_edges) {
			if(e->a() == AExclusive || e->b() == AExclusive) {
				e->set_exists(false);
			} else if(!not_in_AExclusive_clust && (e->a() == lastAdded || e->b() == lastAdded)) {
				not_in_AExclusive_clust = e;
			}
		}
		AExclusive->set_exists(false);
		if(!not_in_AExclusive_clust) {
			return A;
		}

		int c = density_A - last_to_Aexclusive->weight() + last_to_Aexclusive->flow_a() + last_to_Aexclusive->flow_b();
		int init_cap = AExclusive->capacity();
		AExclusive->set_capacity(0);
		A->distribute(last_to_Aexclusive);
		if(A->density_around(lastAdded) > c) {
			return induced_graph(labeled_clusters());
		}

		AExclusive->set_capacity(init_cap);
		for(edge_ptr e : mSubClusters_edges) {
			e->set_exists(true);
		}
		AExclusive->set_exists(true);
		B->add_cluster(AExclusive);
	}

	if(density_B > 0) {
		return B;
	}
	return A;
}
bool cluster::dense(int k, std::vector<cluster_ptr>& outDense, cluster_ptr& lastAdded)
{
	if(k >= 0) {
		return dense_k_positive(k, outDense, lastAdded);
	} else {
		return dense_k_negative(k, outDense, lastAdded);
	}
}
int cluster::distribute(edge_ptr edg)
{
	if(!edg->exists()) {
		LOG_WARNING("Trying to distribute edge that does not exist.");
		return density();
	}

	reset_childrenMarquers();
	reset_childrenPaths();

	cluster_ptr greatestCapcityCluster = nullptr;
	int greatestCapacity = 0;

	edg->set_label();
	edg->set_capacity(edg->weight());
	while(edg->weight() > (edg->flow_a() + edg->flow_b()) || has_labeled_unscanned()) {
		for(edge_ptr currEdg : mSubClusters_edges) {
			if(!currEdg->exists())
				continue;
			if(currEdg->label() && !currEdg->scan()) {
				currEdg->a()->set_label();
				currEdg->a()->set_prevEdge(currEdg);
				currEdg->a()->set_capacity(currEdg->capacity());

				currEdg->b()->set_label();
				currEdg->b()->set_prevEdge(currEdg);
				currEdg->b()->set_capacity(currEdg->capacity());
				currEdg->set_scan();
			}
		}
		for(cluster_ptr clust : mSubClusters) {
			if(!clust->exists() || !(clust->label() && !clust->scan()))
				continue;
			int clusterCapacity = std::min(clust->weight() - clust->sum_incidentEdges_capacity(), clust->capacity());
			if(clusterCapacity > greatestCapacity) {
				greatestCapcityCluster = clust;
				greatestCapacity = clusterCapacity;
			} else {
				label_incidentEdges(clust);
			}
			clust->set_scan();
		}
		if(greatestCapcityCluster) {
			edge_ptr augmentingPath_edge = greatestCapcityCluster->prevEdge();
			if(!augmentingPath_edge) {
				LOG_ERROR("No valid augmentation path.");
			}
	
			int modifier = 0;
			if(augmentingPath_edge->a() == greatestCapcityCluster) {
				modifier = std::min(greatestCapcityCluster->weight() - augmentingPath_edge->flow_a(), augmentingPath_edge->capacity());
				augmentingPath_edge->incr_flow_a(modifier);
			} else if(augmentingPath_edge->b() == greatestCapcityCluster) {
				modifier = std::min(greatestCapcityCluster->weight() - augmentingPath_edge->flow_b(), augmentingPath_edge->capacity());
				augmentingPath_edge->incr_flow_b(modifier);
			} else {
				LOG_WARNING("Should not be reached but ok..");
			}
			greatestCapcityCluster->set_capacity(greatestCapcityCluster->capacity() - modifier);
			reset_childrenMarquers();
			greatestCapcityCluster = nullptr;
			greatestCapacity = 0;
			edg->set_label();
			edg->set_capacity(edg->weight() - (edg->flow_a() + edg->flow_b()));
		}		
	}

	int labeled_density = 0;
	for(edge_ptr e : mSubClusters_edges) {
		if(e->a()->label() && e->b()->label())
			labeled_density += e->weight();
	}
	for(cluster_ptr clust : mSubClusters) {
		if(clust->label())
			labeled_density -= clust->weight();
	}

	return labeled_density;
}

void cluster::add_cluster(cluster_ptr clust)
{
	set_childrenLabels(2, false);
	label_incidentEdges(2);
	if(clust->label() != 2) {
		mSubClusters.push_back(clust);
		// mDensity -= clust->density();
		for(edge_ptr edg : clust->incidentEdges()) {
			if((edg->a()->label() == 2 || edg->b()->label() == 2)) {
				mSubClusters_edges.push_back(edg);
				// mDensity += edg->weight();
				// edg->set_label(2);
			} 
			if(edg->label() == 2) {
				mIncidentEdges.erase(std::find(mIncidentEdges.begin(), mIncidentEdges.end(), edg));
			} else if(edg->label() != 2) {
				mIncidentEdges.push_back(edg);
			}
		}
	}
	set_childrenLabels(0, true);
	compute_density();
}

int cluster::sum_incidentEdges_capacity(bool labeled_only)
{
	int running_sum = 0;
	for(edge_ptr edg : mIncidentEdges) {
		if(!labeled_only || edg->label())
			running_sum += edg->capacity();
	}
	return running_sum;
}
int cluster::sum_incidentEdges_weight(bool labeled_only)
{
	int running_sum = 0;
	for(edge_ptr edg : mIncidentEdges) {
		if(!labeled_only || edg->label())
			running_sum += edg->weight();
	}
	return running_sum;
}

int cluster::incidentEdges_density()
{
	int running_sum = 0;
	for(edge_ptr edg : mIncidentEdges) {
		running_sum += -edg->weight() + edg->flow_a() + edg->flow_b();
	}
	return running_sum;
}

void cluster::label_incidentEdges(cluster_ptr clust, bool with_flow)
{
	for(edge_ptr edg : clust->mIncidentEdges) {
		if((edg->a() == clust && (with_flow || edg->flow_a() > 0)) || (edg->a() == clust && (with_flow || edg->flow_a() > 0)))
			edg->set_label();
	}
}
void cluster::label_incidentEdges(int val)
{
	for(edge_ptr edg : mIncidentEdges) {
		edg->set_label(val);
	}
}
void cluster::label_allNodes_recursive(int val)
{
	for(edge_ptr edg : mSubClusters_edges) {
		edg->set_label(val);
	}

	for(cluster_ptr clus : mSubClusters) {
		clus->set_label(val);
		clus->label_allNodes_recursive(val);
	}
}

void cluster::add_incidentEdge(edge_ptr e)
{
	for(edge_ptr edg : mIncidentEdges) {
		if(edg == e) 
			return;
	}
	mIncidentEdges.push_back(e);
}

int cluster::density_around(cluster_ptr subClust)
{
	if(!subClust->exists())
		return 0;
	int running_sum = 0;
	for(edge_ptr edg : mSubClusters_edges) {
		if(edg->exists() && edg->incident(subClust)) {
			running_sum += -edg->weight() + edg->flow_a() + edg->flow_b();
		}
	}
	return running_sum;
}

bool cluster::has_labeled_unscanned()
{
	for(cluster_ptr clust : mSubClusters) {
		if(clust->label() && !clust->scan() && clust->exists())
			return true;
	}
	for(edge_ptr edg : mSubClusters_edges) {
		if(edg->label() && !edg->scan() && edg->exists())
			return true;
	}
	return false;
}

void cluster::set_available_for_dense_recursive(bool available)
{
	set_available_for_dense(available);
	for(cluster_ptr clus : mSubClusters) {
		clus->set_available_for_dense_recursive(available);
	}
}


cluster_ptr cluster::induced_graph(std::vector<cluster_ptr> clusters)
{
	reset_childrenMarquers();
	for(cluster_ptr clust : clusters) {
		clust->set_label();
	}

	std::vector<edge_ptr> edges;
	for(edge_ptr e : mSubClusters_edges) {
		if(!e->exists())
			continue;
		if(e->a()->label() && e->b()->label())
			edges.push_back(e);
	}
	cluster_ptr graph(new cluster(0));
	graph->mSubClusters = clusters;
	graph->mSubClusters_edges = edges;
	graph->compute_density();
	return graph;
}

std::vector<cluster_ptr> cluster::labeled_clusters()
{
	std::vector<cluster_ptr> out;	
	for(cluster_ptr clust : mSubClusters) {
		if(clust->label())
			out.push_back(clust);
	}
	return out;
}

void cluster::reroute_incidentEdges()
{
	label_allNodes_recursive(2);
	std::vector<edge_ptr> newIncident;
	for(edge_ptr edg : mIncidentEdges) {
		cluster_ptr shared_this = shared_from_this();
		if(!edg->incident(shared_this)) {
			int lab_a = edg->a()->label(), lab_b = edg->b()->label();			
			if(lab_a == 2 && lab_b != 2) {
				edge_ptr newEdge(new edge(shared_this, edg->b(), edg->weight()));
				newIncident.push_back(newEdge);
				newEdge->set_endPointIncidence();
			} else if(lab_a != 2 && lab_b == 2) {
				edge_ptr newEdge(new edge(shared_this, edg->a(), edg->weight()));
				newIncident.push_back(newEdge);
				newEdge->set_endPointIncidence();
			}
		}
	}
	mIncidentEdges = newIncident;
	label_allNodes_recursive(0);
}

void cluster::for_each(std::function<void (cluster_ptr c)> func)
{
	for(cluster_ptr clus : mSubClusters) {
		func(clus);
	}
}

void cluster::reset_childrenMarquers()
{
	for(cluster_ptr clus : mSubClusters) {
		clus->reset_marquers();
		for(edge_ptr edg : clus->incidentEdges()) {
			edg->reset_marquers();
		} 
	}
}
void cluster::set_childrenLabels(int val, bool incidentEdges_)
{
	for(cluster_ptr clus : mSubClusters) {
		clus->set_label(val);
		if(incidentEdges_) {
			for(edge_ptr edg : clus->incidentEdges()) {
				edg->set_label(val);
			}
		}
	}
}
void cluster::reset_childrenPaths()
{
	for(cluster_ptr clus : mSubClusters) {
		clus->set_prevEdge(nullptr);
	}	
}

int cluster::compute_density()
{
	mDensity = mWeight;

	for(cluster_ptr clust : mSubClusters) {
		mDensity -= clust->weight();
	}
	for(edge_ptr edg : mSubClusters_edges) {
		mDensity += edg->weight();
	}
	return mDensity;
}

bool cluster::dense_k_positive(int k, std::vector<cluster_ptr>& outDense, cluster_ptr& lastAdded)
{
	std::vector<cluster_ptr> G_;
	for(cluster_ptr clust : mSubClusters) {
		if(!clust->available_for_dense())
			continue;
		for(edge_ptr edg : mSubClusters_edges) {
			if(!edg->incident(G_) || !edg->incident(clust))
				continue;
			if(distribute(edg) > k) {
				outDense = labeled_clusters();
				lastAdded = G_.back();
				return true;
			}
		}
		G_.push_back(clust);
	}
	return false;
}
bool cluster::dense_k_negative(int k, std::vector<cluster_ptr>& outDense, cluster_ptr& lastAdded)
{
	std::vector<cluster_ptr> G_;
	for(cluster_ptr clust : mSubClusters) {
		if(!clust->available_for_dense())
			continue;
		for(edge_ptr edg : mSubClusters_edges) {
			if(!edg->incident(G_) || !edg->incident(clust))
				continue;
			if(clust->weight() + k >= 0) {
				int init_cap = clust->capacity();
				clust->set_capacity(clust->weight() + k);
				if(distribute(edg) > k) {
					outDense = labeled_clusters();
					lastAdded = G_.back();
					return true;
				}
				clust->set_capacity(init_cap);
			} else {
				// int init_v_cap = v->capacity;
				clust->set_capacity(0);
				if(distribute(edg) > k) {
					outDense = labeled_clusters();
					lastAdded = G_.back();
					return true;
				}
				int init_cap = edg->capacity();
				edg->set_capacity(edg->capacity() - (clust->weight() + k));
				if(distribute(edg) > k) {
					outDense = labeled_clusters();
					lastAdded = G_.back();
					return true;
				}
				edg->set_capacity(init_cap);
			}
		}
		G_.push_back(clust);
	}
	return false;
}



edge::edge(std::shared_ptr<cluster> a_, std::shared_ptr<cluster> b_, int w):
	mA(a_),
	mB(b_),
	mPrevVert(nullptr),
	mWeight(w),
	mCapacity(0),
	mFlow_a(0),
	mFlow_b(0),
	mLabel(0),
	mScan(0),
	mExists(true),
	mName(++counter)
{
	
}

void edge::set_endPointIncidence()
{
	mA->add_incidentEdge(shared_from_this());
	mB->add_incidentEdge(shared_from_this());	
}

bool edge::incident(cluster_ptr clust)
{
	return exists() && clust->exists() && (mA == clust || mB == clust);
}
bool edge::incident(std::vector<cluster_ptr> graph)
{
	if(!exists())
		return false;
	for(cluster_ptr clust : graph) {
		if(clust->exists() && (mA == clust || mB == clust))
			return true;
	}
	return false;
}