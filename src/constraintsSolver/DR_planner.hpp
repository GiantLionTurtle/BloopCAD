
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
private:
	std::vector<cluster_ptr> mSubClusters;
	std::vector<edge_ptr> mSubClusters_edges;
	std::vector<edge_ptr> mIncidentEdges;
	edge_ptr mPrevEdge;
	int mWeight, mCapacity, mDensity;
	int mLabel, mScan;
	bool mExists;
	int mName;
	static int counter;
public:
	cluster(int weight_);
	cluster(std::vector<cluster_ptr> clusters, cluster_ptr base = nullptr);
	cluster(std::vector<cluster_ptr> clusters, std::vector<edge_ptr> edges, cluster_ptr base = nullptr);

	cluster_ptr simplify(int k);
	cluster_ptr extend(cluster_ptr min);
	cluster_ptr minimal(int k);
	bool dense(int k, std::vector<cluster_ptr>& outDense, cluster_ptr& lastAdded);
	int distribute(edge_ptr e);

	void set_prevEdge(edge_ptr e) { mPrevEdge = e; }
	edge_ptr prevEdge() { return mPrevEdge; }

	std::vector<cluster_ptr> subClusters() { return mSubClusters; }
	void add_cluster(cluster_ptr clust);

	int num_incidentEdges() const { return mIncidentEdges.size(); }
	edge_ptr incidentEdge(unsigned int ind) { return ind < mIncidentEdges.size() ? mIncidentEdges[ind] : nullptr; }
	std::vector<edge_ptr> incidentEdges() { return mIncidentEdges; }
	int sum_incidentEdges_capacity(bool labeled_only = false);
	int sum_incidentEdges_weight(bool labeled_only = false);
	int incidentEdges_density();
	void label_incidentEdges(cluster_ptr clust, bool with_flow = false);
	void label_incidentEdges();
	void add_incidentEdge(edge_ptr e);
	void clear_incidentEdges() { mIncidentEdges.clear(); }

	int weight() const { return mWeight; }
	void set_weight(int w) { mWeight = w; }

	int capacity() const { return mCapacity; }
	void set_capacity(int c) { mCapacity = c; }

	int density() const { return mDensity; }
	int density_around(cluster_ptr subClust);

	int label() const { return mLabel; }
	int scan() const { return mScan; }
	void set_label(int l = 1) { mLabel = l; }
	void set_scan(int s = 1) { mScan = 1; }
	void reset_label() { mLabel = 0; }
	void reset_scan() { mScan = 0; }
	void reset_marquers() { mLabel = 0; mScan = 0; }
	bool has_labeled_unscanned();


	bool exists() const { return mExists; }
	void set_exists(bool e) { mExists = e; }

	int name() const { return mName; }

	cluster_ptr induced_graph(std::vector<cluster_ptr> clusters);
	std::vector<cluster_ptr> labeled_clusters();
private:
	void for_each(std::function<void (cluster_ptr c)> func);
	void reset_childrenMarquers();
	void set_childrenLabels(int val);
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
	int mLabel, mScan;
	bool mExists;
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

	int label() const { return mLabel; }
	int scan() const { return mScan; }
	void set_label(int l = 1) { mLabel = l; }
	void set_scan(int s = 1) { mScan = 1; }
	void reset_label() { mLabel = 0; }
	void reset_scan() { mScan = 0; }
	void reset_marquers() { mLabel = 0; mScan = 0; }

	bool exists() const { return mExists; }
	void set_exists(bool e) { mExists = e; }

	int name() const { return mName; }

	void set_endPointIncidence();

	bool incident(cluster_ptr clust);
	bool incident(std::vector<cluster_ptr> clust);
};

#endif