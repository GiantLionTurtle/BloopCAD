
#ifndef DR_PLANNER_HPP_
#define DR_PLANNER_HPP_

// https://www.cs.purdue.edu/homes/cmh/distribution/papers/Constraints/CP97.pdf
// https://www.cs.purdue.edu/homes/cmh/distribution/

#include <memory>
#include <vector>
#include <map>
#include <functional>
#include "equationsSystem.hpp"

class cluster;
class constraint;
using cluster_ptr = std::shared_ptr<cluster>;
using constraint_ptr = std::shared_ptr<constraint>;


class cluster {
public:
	enum labels { DENSE_LABEL = 1, DENSE_SCAN = 2, EXISTS = 4, DENSE_USABLE = 8, MISC = 16, INCIDENT = 32 };
protected:
	equationsSystem* mNumericalSolver;

	std::vector<cluster_ptr> mSubClusters;
	std::vector<constraint_ptr> mSubClusters_constraints;
	constraint_ptr mPrevEdge;
	int mWeight, mCapacity, mDensity;
	int mSuperLabel;
	int mName;
	bool mSolver_upToDate;
	static int counter;
public:
	cluster(int weight_);
	cluster(std::vector<cluster_ptr> clusters, cluster* base = nullptr);
	cluster(std::vector<cluster_ptr> clusters, std::vector<constraint_ptr> constraints, cluster* base = nullptr);
	virtual ~cluster();

	int solve();

	std::vector<cluster_ptr> subClusters() { return mSubClusters; }
	void add_cluster(cluster_ptr clust, std::vector<constraint_ptr> incidentEdges);
	void add_cluster(cluster_ptr clust);
	void add_constraint(constraint_ptr edg);

	int weight() const { return mWeight; }
	void set_weight(int w) { mWeight = w; }

	int capacity() const { return mCapacity; }
	void set_capacity(int c) { mCapacity = c; }

	int density() const { return mDensity; }
	int density_around(cluster_ptr subClust);

	bool has_label(int lab) const { return mSuperLabel & lab; }

	void add_label(int lab) { mSuperLabel |= lab; }
	void add_label_incidentEdges(cluster_ptr clust, int lab, bool with_flow = false);
	void add_label_recursive(int lab, bool skip_self = false, bool subconstraints = false);
	void add_label_children(int lab, bool subconstraints = false);
	void add_label_constraints(int lab);

	void remove_label(int lab) { mSuperLabel &= ~lab; };
	void remove_label_incidentEdges(cluster_ptr clust, int lab);
	void remove_label_recursive(int lab, bool skip_self = false, bool subconstraints = false);
	void remove_label_children(int lab, bool subconstraints = false);
	void remove_label_constraints(int lab);
	
	bool has_labeled_unscanned();

	cluster_ptr induced_graph(std::vector<cluster_ptr> clusters);
	std::vector<cluster_ptr> labeled_clusters();
private:
	cluster_ptr skeletonize(std::vector<cluster_ptr> denseSubClusters);
	std::vector<cluster_ptr> denseClusters(int k);
	cluster_ptr find_denseCluster(std::vector<cluster_ptr>& root_mask, int k);
	cluster_ptr simplify(int k);
	cluster_ptr reduce(int k);
	cluster_ptr extend(cluster_ptr min);
	cluster_ptr minimal(int k);
	cluster_ptr minimal(int k, std::vector<cluster_ptr> const& available_clusters);
	bool dense(int k, std::vector<cluster_ptr>& outDense, cluster_ptr& lastAdded);
	bool dense(int k, std::vector<cluster_ptr>& outDense, cluster_ptr& lastAdded, std::vector<cluster_ptr> const& available_clusters);
	int distribute(constraint_ptr e);

	std::vector<constraint_ptr> incidentEdges(cluster_ptr clust);
	int sum_incidentEdges_capacity(cluster_ptr clust, int labeled_only = 0);
	int sum_incidentEdges_weight(cluster_ptr clust, int labeled_only = 0);


	void set_prevEdge(constraint_ptr e) { mPrevEdge = e; }
	constraint_ptr prevEdge() { return mPrevEdge; }

	void for_each_cluster(std::function<void (cluster_ptr c)> func);
	void reset_childrenPaths();

	int compute_density();

	bool dense_k_positive(int k, std::vector<cluster_ptr>& outDense, cluster_ptr& lastAdded);
	bool dense_k_negative(int k, std::vector<cluster_ptr>& outDense, cluster_ptr& lastAdded);

	equationsSystem* create_solver();
	void update_solver(equationsSystem* solver);
};

class constraint {
private:
	cluster_ptr mA, mB; // The two clusters linked by the constraint
	cluster_ptr mPrevVert; // The previous cluster in the augmenting path
	int mWeight;
	int mCapacity;
	int mFlow_a, mFlow_b;
	int mSuperLabel;
	int mName;
	static int counter;
public:
	constraint(cluster_ptr a_, cluster_ptr b_, int w);

	cluster_ptr a() { return mA; }
	cluster_ptr b() { return mB; }

	int weight() const { return mWeight; }
	void set_weight(int w) { mWeight = w; }

	int capacity() const { return mCapacity; }
	void set_capacity(int c) { mCapacity = c; }

	int flow_a() const { return mFlow_a; }
	int flow_b() const { return mFlow_b; }
	void set_flow_a(int flow) { mFlow_a = flow; }
	void set_flow_b(int flow) { mFlow_b = flow; }
	void incr_flow_a(int flow) { mFlow_a += flow; }
	void incr_flow_b(int flow) { mFlow_b += flow; }

	bool has_label(int lab) const { return mSuperLabel & lab; }
	void add_label(int lab) { mSuperLabel |= lab; }
	void remove_label(int lab) { mSuperLabel &= ~lab; };

	int name() const { return mName; }

	// void set_endPointIncidence();

	bool incident(cluster_ptr clust);
	bool incident(std::vector<cluster_ptr> clust);

	virtual void add_equations_to_system(equationsSystem* sys) {};

	void add_equation_to_system(equationsSystem* sys, expression_ptr expr);
};

#endif