
#ifndef DR_PLANNER_HPP_
#define DR_PLANNER_HPP_

// https://www.cs.purdue.edu/homes/cmh/distribution/papers/Constraints/CP97.pdf

#include <memory>
#include <vector>
#include <functional>

// struct vertex;
class cluster;
class edge;
class graph;
// using vertex_ptr = std::shared_ptr<vertex>;
using cluster_ptr = std::shared_ptr<cluster>;
using edge_ptr = std::shared_ptr<edge>;
using graph_ptr = std::shared_ptr<graph>;

class cluster : public std::enable_shared_from_this<cluster> {
public:
	enum labels { DENSE_LABEL = 1, DENSE_SCAN = 2, EXISTS = 4, DENSE_USABLE = 8, MISC = 16 };
private:
	std::vector<cluster_ptr> mSubClusters;
	std::vector<edge_ptr> mSubClusters_edges;
	std::vector<edge_ptr> mIncidentEdges;
	edge_ptr mPrevEdge;
	int mWeight, mCapacity, mDensity;
	int mSuperLabel;
	int mName;
	static int counter;
public:
	cluster(int weight_);
	cluster(std::vector<cluster_ptr> clusters, cluster* base = nullptr);
	cluster(std::vector<cluster_ptr> clusters, std::vector<edge_ptr> edges, cluster* base = nullptr);

	std::vector<cluster_ptr> denseClusters(int k);
	cluster_ptr find_denseCluster(std::vector<cluster_ptr>& root_mask, int k);
	cluster_ptr simplify(int k);
	cluster_ptr reduce(int k);
	cluster_ptr extend(cluster_ptr min);
	cluster_ptr minimal(int k);
	cluster_ptr minimal(int k, std::vector<cluster_ptr> const& available_clusters);
	bool dense(int k, std::vector<cluster_ptr>& outDense, cluster_ptr& lastAdded);
	bool dense(int k, std::vector<cluster_ptr>& outDense, cluster_ptr& lastAdded, std::vector<cluster_ptr> const& available_clusters);
	int distribute(edge_ptr e);

	void set_prevEdge(edge_ptr e) { mPrevEdge = e; }
	edge_ptr prevEdge() { return mPrevEdge; }

	std::vector<cluster_ptr> subClusters() { return mSubClusters; }
	void add_cluster(cluster_ptr clust);

	int num_incidentEdges() const { return mIncidentEdges.size(); }
	edge_ptr incidentEdge(unsigned int ind) { return ind < mIncidentEdges.size() ? mIncidentEdges[ind] : nullptr; }
	std::vector<edge_ptr> incidentEdges() { return mIncidentEdges; }
	int sum_incidentEdges_capacity(int labeled_only = 0);
	int sum_incidentEdges_weight(int labeled_only = 0);
	int incidentEdges_density();
	void label_incidentEdges(cluster_ptr clust, bool with_flow = false);
	void label_incidentEdges(int val = 1);
	void add_incidentEdge(edge_ptr e);
	void clear_incidentEdges() { mIncidentEdges.clear(); }

	int weight() const { return mWeight; }
	void set_weight(int w) { mWeight = w; }

	int capacity() const { return mCapacity; }
	void set_capacity(int c) { mCapacity = c; }

	int density() const { return mDensity; }
	int density_around(cluster_ptr subClust);


	bool has_label(int lab) const { return mSuperLabel & lab; }

	void add_label(int lab) { mSuperLabel |= lab; }
	void add_label_incidentEdges(int lab);
	void add_label_recursive(int lab, bool skip_self = false, bool subedges = false);
	void add_label_children(int lab, bool subedges = false);
	void add_label_edges(int lab);

	void remove_label(int lab) { mSuperLabel &= ~lab; };
	void remove_label_incidentEdges(int lab);
	void remove_label_recursive(int lab, bool skip_self = false, bool subedges = false);
	void remove_label_children(int lab, bool subedges = false);
	void remove_label_edges(int lab);
	
	bool has_labeled_unscanned();

	int name() const { return mName; }

	cluster_ptr induced_graph(std::vector<cluster_ptr> clusters);
	std::vector<cluster_ptr> labeled_clusters();

	void reroute_incidentEdges();
private:
	void for_each(std::function<void (cluster_ptr c)> func);
	void reset_childrenPaths();

	int compute_density();

	bool dense_k_positive(int k, std::vector<cluster_ptr>& outDense, cluster_ptr& lastAdded);
	bool dense_k_negative(int k, std::vector<cluster_ptr>& outDense, cluster_ptr& lastAdded);
};

class edge : public std::enable_shared_from_this<edge> {
private:
	cluster_ptr mA, mB; // The two clusters linked by the edge
	cluster_ptr mPrevVert; // The previous cluster in the augmenting path
	int mWeight;
	int mCapacity;
	int mFlow_a, mFlow_b;
	int mSuperLabel;
	int mName;
	static int counter;
public:
	edge(cluster_ptr a_, cluster_ptr b_, int w);

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

	void set_endPointIncidence();

	bool incident(cluster_ptr clust);
	bool incident(std::vector<cluster_ptr> clust);
};

#endif