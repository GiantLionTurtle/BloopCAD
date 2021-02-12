
#include "DR_planner.hpp"

#include <utils/errorLogger.hpp>

#include <cmath>
#include <limits>
#include <algorithm>

int cluster::counter = 0;
int constraint::counter = 0;

cluster::cluster(int w):
	mNumericalSolver(nullptr),
	mSubClusters(0),
	mSubClusters_constraints(0),
	mPrevEdge(nullptr),
	mWeight(w),
	mCapacity(0),
	mDensity(w),
	mSuperLabel(labels::DENSE_USABLE | labels::EXISTS),
	mSolver_upToDate(false),
	mName(++counter)
{

}
cluster::cluster(std::vector<cluster_ptr> clusters, cluster* base):
	mNumericalSolver(nullptr),
	mSubClusters(clusters),
	mSubClusters_constraints(0),
	mPrevEdge(nullptr),
	mWeight(0),
	mCapacity(0),
	mDensity(0),
	mSuperLabel(labels::DENSE_USABLE | labels::EXISTS),
	mSolver_upToDate(false),
	mName(++counter)
{
	add_label_children(labels::MISC, true);
	add_label_constraints(labels::MISC);

	if(base) {
		for(cluster_ptr clus : mSubClusters) {
			for(constraint_ptr edg : base->incidentEdges(clus)) {
				if(edg->a()->has_label(labels::MISC) && edg->b()->has_label(labels::MISC) && !edg->has_label(labels::MISC)) {
					edg->add_label(labels::MISC);
					mSubClusters_constraints.push_back(edg);
				}
			}
		}
	}

	remove_label_children(labels::MISC, true);
	remove_label_constraints(labels::MISC);
	compute_density();
}
cluster::cluster(std::vector<cluster_ptr> clusters, std::vector<constraint_ptr> constraints, cluster* base):
	mNumericalSolver(nullptr),
	mSubClusters(clusters),
	mSubClusters_constraints(0),
	mPrevEdge(nullptr),
	mWeight(0),
	mCapacity(0),
	mDensity(0),
	mSuperLabel(labels::DENSE_USABLE | labels::EXISTS),
	mSolver_upToDate(false),
	mName(++counter)
{
	add_label_children(labels::MISC, true);
	add_label_constraints(labels::MISC);

	for(constraint_ptr edg : constraints) {
		if(edg->a()->has_label(labels::MISC) && edg->b()->has_label(labels::MISC) && !edg->has_label(labels::MISC)) {
			edg->add_label(labels::MISC);
			mSubClusters_constraints.push_back(edg);
		}
	}

	remove_label_children(labels::MISC, true);
	remove_label_constraints(labels::MISC);
	compute_density();
}
cluster::~cluster()
{
	if(mNumericalSolver)
		delete mNumericalSolver;
}

int cluster::solve()
{
	if(!mNumericalSolver) {
		mNumericalSolver = create_solver();
		mSolver_upToDate = true;
	}
	if(!mSolver_upToDate) {

	}
	return mNumericalSolver->solve();
}

cluster_ptr cluster::skeletonize(std::vector<cluster_ptr> denseSubClusters)
{
	std::vector<cluster_ptr> intersections;
	std::vector<constraint_ptr> constraints; 

	for(int i = 0; i < denseSubClusters.size()-1; ++i) {
		cluster_ptr outer_cluster = denseSubClusters[i];
		outer_cluster->add_label_children(labels::MISC);
		for(int j = i+1; j < denseSubClusters.size(); ++j) {
			for(cluster_ptr candidate : denseSubClusters[j]->subClusters()) {
				if(candidate->has_label(labels::MISC))
					intersections.push_back(candidate);
			}
			
		}
		outer_cluster->remove_label_children(labels::MISC);
	}

	for(cluster_ptr denseClust : denseSubClusters) {
		std::vector<cluster_ptr> X;
		denseClust->add_label_children(labels::MISC);
		for(cluster_ptr inter : intersections) {
			if(inter->has_label(labels::MISC))
				X.push_back(inter);
		}
		denseClust->remove_label_children(labels::MISC);

		if(X.size() >= 2) {
			constraints.push_back(constraint_ptr(new constraint(X[0], X[1], 0)));
		}
		if(X.size() > 2) {
			for(int i = 2; i < X.size(); ++i) {
				constraints.push_back(constraint_ptr(new constraint(X[i], X[i-1], 0)));
				constraints.push_back(constraint_ptr(new constraint(X[i], X[i-2], 0)));
			}
		}
	}
	return cluster_ptr(new cluster(intersections, constraints));
}
std::vector<cluster_ptr> cluster::denseClusters(int k)
{
	std::vector<cluster_ptr> out;
	cluster_ptr subCluster = nullptr;
	while((subCluster = find_denseCluster(out, k))) {
		out.push_back(subCluster);
	}
	return out;
}

cluster_ptr cluster::find_denseCluster(std::vector<cluster_ptr>& root_mask, int k)
{
	for(cluster_ptr clust : root_mask) {
		clust->remove_label_recursive(labels::DENSE_USABLE);
	}

	cluster_ptr out = extend(minimal(-4));

	for(cluster_ptr clust : root_mask) {
		clust->add_label_recursive(labels::DENSE_USABLE);
	}
	return out;
}

cluster_ptr cluster::simplify(int k)
{
	cluster_ptr simplified = reduce(k);
	cluster_ptr tmp_simplified = nullptr;
	while(simplified->mSubClusters.size() > 1) {
		tmp_simplified = simplified->reduce(k);
		if(!tmp_simplified) {
			LOG_WARNING("Could not simplify to single node.");
			break;
		}
		simplified = tmp_simplified;
	}
	return simplified;
}
cluster_ptr cluster::reduce(int k) 
{
	cluster_ptr subgraph = extend(minimal(k));
	if(!subgraph)
		return nullptr;
	subgraph->set_weight(-(k+1));
	remove_label_recursive(labels::MISC, true, true);
	subgraph->add_label_recursive(labels::MISC, true);
	std::vector<cluster_ptr> reduced_clusters(mSubClusters.size() - subgraph->mSubClusters.size() + 1);
	int ind = 0;
	for(cluster_ptr clust : mSubClusters) {
		if(!clust->has_label(labels::MISC)) {
			reduced_clusters[ind++] = clust;
		}
	}
	reduced_clusters[ind++] = subgraph;
	subgraph->remove_label_recursive(labels::MISC, true, true);
	// subgraph->reroute_incidentEdges();
	return std::make_shared<cluster>(reduced_clusters, this);
}
cluster_ptr cluster::extend(cluster_ptr min)
{
	if(!min)
		return nullptr;
	cluster_ptr newGraph(new cluster(min->mSubClusters, min->mSubClusters_constraints, this));
	int init_size = 0;
	remove_label_recursive(labels::MISC, false, true);
	while(newGraph->mSubClusters.size() > init_size) {
		init_size = newGraph->mSubClusters.size();
		newGraph->add_label_children(labels::MISC, true);
		add_label_incidentEdges(newGraph, labels::MISC);

		for(cluster_ptr clust : mSubClusters) {
			if(!clust->has_label(labels::MISC) && sum_incidentEdges_weight(clust, labels::MISC) >= clust->weight()) {
				newGraph->add_cluster(clust, incidentEdges(clust));
			}
		}
	}
	remove_label_recursive(labels::MISC, false, true);
	return newGraph;
}
cluster_ptr cluster::minimal(int k)
{
	std::vector<cluster_ptr> dense_clusters;
	cluster_ptr lastAdded;
	if(!dense(k, dense_clusters, lastAdded)) 
		return nullptr;
	
	cluster_ptr A(new cluster(dense_clusters, this));
	cluster_ptr B(new cluster({ lastAdded }));

	int density_A;
	int density_B;
	while((density_B = B->density() <= 0 && (density_A = A->density_around(lastAdded)) > (/*k+*/1))) {
		cluster_ptr AExclusive = nullptr;
		constraint_ptr last_to_Aexclusive = nullptr;
		for(constraint_ptr e : mSubClusters_constraints) {
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
		constraint_ptr not_in_AExclusive_clust = nullptr;
		for(constraint_ptr e : mSubClusters_constraints) {
			if(e->a() == AExclusive || e->b() == AExclusive) {
				e->remove_label(labels::EXISTS);
			} else if(!not_in_AExclusive_clust && (e->a() == lastAdded || e->b() == lastAdded)) {
				not_in_AExclusive_clust = e;
			}
		}
		AExclusive->remove_label(labels::EXISTS);
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
		for(constraint_ptr e : mSubClusters_constraints) {
			e->add_label(labels::EXISTS);
		}
		AExclusive->add_label(labels::EXISTS);
		B->add_cluster(AExclusive, incidentEdges(AExclusive));
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
int cluster::distribute(constraint_ptr edg)
{
	if(!edg->has_label(labels::EXISTS)) {
		LOG_WARNING("Trying to distribute constraint that does not exist.");
		return density();
	}

	remove_label_recursive(labels::DENSE_LABEL | labels::DENSE_SCAN, false, true);
	reset_childrenPaths();

	cluster_ptr greatestCapcityCluster = nullptr;
	int greatestCapacity = 0;

	edg->add_label(labels::DENSE_LABEL);
	edg->set_capacity(edg->weight());
	while(edg->weight() > (edg->flow_a() + edg->flow_b()) || has_labeled_unscanned()) {
		for(constraint_ptr currEdg : mSubClusters_constraints) {
			if(!currEdg->has_label(labels::EXISTS))
				continue;
			if(currEdg->has_label(labels::DENSE_LABEL) && !currEdg->has_label(labels::DENSE_SCAN)) {
				currEdg->a()->add_label(labels::DENSE_LABEL);
				currEdg->a()->set_prevEdge(currEdg);
				currEdg->a()->set_capacity(currEdg->capacity());

				currEdg->b()->add_label(labels::DENSE_LABEL);
				currEdg->b()->set_prevEdge(currEdg);
				currEdg->b()->set_capacity(currEdg->capacity());
				currEdg->add_label(labels::DENSE_SCAN);
			}
		}
		for(cluster_ptr clust : mSubClusters) {
			if(!clust->has_label(labels::EXISTS) || !(clust->has_label(labels::DENSE_LABEL) && !clust->has_label(labels::DENSE_SCAN)))
				continue;
			int clusterCapacity = std::min(clust->weight() - sum_incidentEdges_capacity(clust), clust->capacity());
			if(clusterCapacity > greatestCapacity) {
				greatestCapcityCluster = clust;
				greatestCapacity = clusterCapacity;
			} else {
				add_label_incidentEdges(clust, labels::DENSE_LABEL, true);
			}
			clust->add_label(labels::DENSE_SCAN);
		}
		if(greatestCapcityCluster) {
			constraint_ptr augmentingPath_constraint = greatestCapcityCluster->prevEdge();
			if(!augmentingPath_constraint) {
				LOG_ERROR("No valid augmentation path.");
			}
	
			int modifier = 0;
			if(augmentingPath_constraint->a() == greatestCapcityCluster) {
				modifier = std::min(greatestCapcityCluster->weight() - augmentingPath_constraint->flow_a(), augmentingPath_constraint->capacity());
				augmentingPath_constraint->incr_flow_a(modifier);
			} else if(augmentingPath_constraint->b() == greatestCapcityCluster) {
				modifier = std::min(greatestCapcityCluster->weight() - augmentingPath_constraint->flow_b(), augmentingPath_constraint->capacity());
				augmentingPath_constraint->incr_flow_b(modifier);
			} else {
				LOG_WARNING("Should not be reached but ok..");
			}
			greatestCapcityCluster->set_capacity(greatestCapcityCluster->capacity() - modifier);
			remove_label_recursive(labels::DENSE_LABEL | labels::DENSE_SCAN, false, true);
			greatestCapcityCluster = nullptr;
			greatestCapacity = 0;
			edg->add_label(labels::DENSE_LABEL);
			edg->set_capacity(edg->weight() - (edg->flow_a() + edg->flow_b()));
		}		
	}

	int labeled_density = 0;
	for(constraint_ptr e : mSubClusters_constraints) {
		if(e->a()->has_label(labels::DENSE_LABEL) && e->b()->has_label(labels::DENSE_LABEL))
			labeled_density += e->weight();
	}
	for(cluster_ptr clust : mSubClusters) {
		if(clust->has_label(labels::DENSE_LABEL))
			labeled_density -= clust->weight();
	}

	return labeled_density;
}

void cluster::add_cluster(cluster_ptr clust, std::vector<constraint_ptr> incidentEdges)
{
	add_label_children(labels::MISC, false);
	if(!clust->has_label(labels::MISC)) {
		mSubClusters.push_back(clust);
		for(constraint_ptr edg : incidentEdges) {
			if(edg->a()->has_label(labels::MISC) || edg->b()->has_label(labels::MISC)) {
				mSubClusters_constraints.push_back(edg);
			} 
		}
	}
	remove_label_children(labels::MISC, false);
	mSolver_upToDate = false;
}

void cluster::add_cluster(cluster_ptr clust)
{
	add_label_children(labels::MISC, false);
	if(!clust->has_label(labels::MISC)) {
		mSubClusters.push_back(clust);
	}
	remove_label_children(labels::MISC, false);
	mSolver_upToDate = false;
}
void cluster::add_constraint(constraint_ptr edg)
{
	add_label_children(labels::MISC, false);
	if(edg->a()->has_label(labels::MISC) && edg->b()->has_label(labels::MISC)) {
		mSubClusters_constraints.push_back(edg);
	}
	remove_label_children(labels::MISC, false);
	mSolver_upToDate = false;
}

std::vector<constraint_ptr> cluster::incidentEdges(cluster_ptr clust)
{
	std::vector<constraint_ptr> out;
	for(constraint_ptr edg : mSubClusters_constraints) {
		if(edg->incident(clust))
			out.push_back(edg);
	}
	return out;
}
int cluster::sum_incidentEdges_capacity(cluster_ptr clust, int labeled_only)
{
	int running_sum = 0;
	for(constraint_ptr edg : mSubClusters_constraints) {
		if(edg->incident(clust) && (!labeled_only || edg->has_label(labeled_only)))
			running_sum += edg->capacity();
	}
	return running_sum;
}
int cluster::sum_incidentEdges_weight(cluster_ptr clust, int labeled_only)
{
	int running_sum = 0;
	for(constraint_ptr edg : mSubClusters_constraints) {
		if(edg->incident(clust) && (!labeled_only || edg->has_label(labeled_only)))
			running_sum += edg->weight();
	}
	return running_sum;
}

int cluster::density_around(cluster_ptr subClust)
{
	if(!subClust->has_label(labels::EXISTS))
		return 0;
	int running_sum = 0;
	for(constraint_ptr edg : mSubClusters_constraints) {
		if(edg->has_label(labels::EXISTS) && edg->incident(subClust)) {
			running_sum += -edg->weight() + edg->flow_a() + edg->flow_b();
		}
	}
	return running_sum;
}

void cluster::add_label_incidentEdges(cluster_ptr clust, int lab, bool with_flow)
{
	clust->add_label_recursive(labels::INCIDENT);
	if(with_flow) {
		for(constraint_ptr edg : mSubClusters_constraints) {
			if(	(edg->a()->has_label(labels::INCIDENT) && (!with_flow || edg->flow_a() > 0)) || 
				(edg->b()->has_label(labels::INCIDENT) && (!with_flow || edg->flow_b() > 0)))
					edg->add_label(lab);
		}
	} else {
		for(constraint_ptr edg : mSubClusters_constraints) {
			if(edg->a()->has_label(labels::INCIDENT) ^ edg->b()->has_label(labels::INCIDENT))
				edg->add_label(lab);
		}
	}
	clust->remove_label_recursive(labels::INCIDENT);
}
void cluster::add_label_recursive(int lab, bool skip_self,  bool subconstraints)
{
	if(!skip_self)
		add_label(lab);
	if(subconstraints)
		add_label_constraints(lab);
	for(cluster_ptr clust : mSubClusters) {
		clust->add_label_recursive(lab, subconstraints);
	}
}
void cluster::add_label_children(int lab,  bool subconstraints)
{
	for(cluster_ptr clust : mSubClusters) {
		clust->add_label(lab);
		if(subconstraints)
			clust->add_label_constraints(lab);
	}
}
void cluster::add_label_constraints(int lab)
{
	for(constraint_ptr edg : mSubClusters_constraints) {
		edg->add_label(lab);
	}
}
void cluster::remove_label_incidentEdges(cluster_ptr clust, int lab)
{
	for(constraint_ptr edg : mSubClusters_constraints) {
		if(edg->incident(clust))
			edg->remove_label(lab);
	}
}
void cluster::remove_label_recursive(int lab, bool skip_self, bool subconstraints)
{
	if(!skip_self)
		remove_label(lab);
	if(!skip_self && subconstraints)
		remove_label_constraints(lab);
	for(cluster_ptr clust : mSubClusters) {
		clust->remove_label_recursive(lab, false, subconstraints);
	}
}
void cluster::remove_label_children(int lab,  bool subconstraints)
{
	for(cluster_ptr clust : mSubClusters) {
		clust->remove_label(lab);
		if(subconstraints)
			clust->remove_label_constraints(lab);
	}
}
void cluster::remove_label_constraints(int lab)
{
	for(constraint_ptr edg : mSubClusters_constraints) {
		edg->remove_label(lab);
	}
}

bool cluster::has_labeled_unscanned()
{
	for(cluster_ptr clust : mSubClusters) {
		if(clust->has_label(labels::DENSE_LABEL) && !clust->has_label(labels::DENSE_SCAN) && clust->has_label(labels::EXISTS))
			return true;
	}
	for(constraint_ptr edg : mSubClusters_constraints) {
		if(edg->has_label(labels::DENSE_LABEL) && !edg->has_label(labels::DENSE_SCAN) && edg->has_label(labels::EXISTS))
			return true;
	}
	return false;
}

cluster_ptr cluster::induced_graph(std::vector<cluster_ptr> clusters)
{
	remove_label_recursive(labels::DENSE_LABEL | labels::DENSE_SCAN);
	for(cluster_ptr clust : clusters) {
		clust->add_label(labels::DENSE_LABEL);
	}

	std::vector<constraint_ptr> constraints;
	for(constraint_ptr e : mSubClusters_constraints) {
		if(!e->has_label(labels::EXISTS))
			continue;
		if(e->a()->has_label(labels::DENSE_LABEL) && e->b()->has_label(labels::DENSE_LABEL))
			constraints.push_back(e);
	}
	cluster_ptr graph(new cluster(0));
	graph->mSubClusters = clusters;
	graph->mSubClusters_constraints = constraints;
	graph->compute_density();
	return graph;
}

std::vector<cluster_ptr> cluster::labeled_clusters()
{
	std::vector<cluster_ptr> out;	
	for(cluster_ptr clust : mSubClusters) {
		if(clust->has_label(labels::DENSE_LABEL))
			out.push_back(clust);
	}
	return out;
}

void cluster::for_each_cluster(std::function<void (cluster_ptr c)> func)
{
	for(cluster_ptr clus : mSubClusters) {
		func(clus);
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
	for(constraint_ptr edg : mSubClusters_constraints) {
		mDensity += edg->weight();
	}
	return mDensity;
}

bool cluster::dense_k_positive(int k, std::vector<cluster_ptr>& outDense, cluster_ptr& lastAdded)
{
	std::vector<cluster_ptr> G_;
	for(cluster_ptr clust : mSubClusters) {
		if(!clust->has_label(labels::DENSE_USABLE))
			continue;
		for(constraint_ptr edg : mSubClusters_constraints) {
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
		if(!clust->has_label(labels::DENSE_USABLE))
			continue;
		for(constraint_ptr edg : mSubClusters_constraints) {
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

equationsSystem* cluster::create_solver()
{
	equationsSystem* sys = new equationsSystem();
	for(constraint_ptr cons : mSubClusters_constraints) {
		cons->add_equations_to_system(sys);
	}
	for(expression_ptr equ : sys->equations()) {
		equ->for_each_var([sys](variable_ptr var) {
			if(!var->tmp_flag) {
				sys->add_variable(var);
				var->tmp_flag = 1;
			}
		});
	}
	for(expression_ptr equ : sys->equations()) {
		equ->for_each_var([](variable_ptr var) {
			var->tmp_flag = 0;
		});
	}
	return sys;
}
void cluster::update_solver(equationsSystem* sys)
{
	for(expression_ptr equ : sys->equations()) {
		equ->tmp_flag = 1;
		equ->for_each_var([](variable_ptr var) {
			var->tmp_flag = 1;
		});
	}
	for(constraint_ptr cons : mSubClusters_constraints) {
		cons->add_equations_to_system(sys);
	}
	for(expression_ptr equ : sys->equations()) {
		equ->for_each_var([sys](variable_ptr var) {
			if(!var->tmp_flag) {
				sys->add_variable(var);
				var->tmp_flag = 1;
			}
		});
	}
	for(expression_ptr equ : sys->equations()) {
		equ->for_each_var([](variable_ptr var) {
			var->tmp_flag = 0;
		});
	}
}

constraint::constraint(std::shared_ptr<cluster> a_, std::shared_ptr<cluster> b_, int w):
	mA(a_),
	mB(b_),
	mPrevVert(nullptr),
	mWeight(w),
	mCapacity(0),
	mFlow_a(0),
	mFlow_b(0),
	mSuperLabel(cluster::labels::EXISTS),
	mName(++counter)
{
	
}

bool constraint::incident(cluster_ptr clust)
{
	return (mA == clust || mB == clust);
}
bool constraint::incident(std::vector<cluster_ptr> graph)
{
	if(!has_label(cluster::labels::EXISTS))
		return false;
	for(cluster_ptr clust : graph) {
		if(clust->has_label(cluster::labels::EXISTS) && (mA == clust || mB == clust))
			return true;
	}
	return false;
}

void constraint::add_equation_to_system(equationsSystem* sys, expression_ptr expr)
{
	if(!expr->tmp_flag)
		sys->add_equation(expr);
}