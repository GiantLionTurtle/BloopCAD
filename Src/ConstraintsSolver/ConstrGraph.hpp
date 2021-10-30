
#ifndef CONSTRGRAPH_HPP_
#define CONSTRGRAPH_HPP_

#include <vector>
#include <set>
#include <stack>
#include <map>
#include <algorithm>

struct Graph {
	struct Vertex {
		int index { -1 };
		int lowLink { -1 };
		bool onStack { false };
	};

	struct Edge {
		Vertex& a;
		Vertex& b;
	};

	int mIndex { 0 };
	std::vector<Vertex> mV;
	std::vector<Edge> mE;
	std::stack<Vertex&> mS;

	std::vector<std::set<Vertex>> tarjan()
	{
		mIndex = 0;
		while(!mS.empty()) {
			mS.pop();
		}
		std::vector<std::set<Vertex>> output;

		for(auto v : mV) {
			if(v.index == -1) {
				strongConnect(v, output);
			}
		}
	}

	void strongConnect(Vertex& v, std::vector<std::set<Vertex>>& out)
	{
		// Set the depth index for v to the smallest unused index
		v.index = mIndex;
		v.lowLink = mIndex;
		mIndex++;
		mS.push(v);
		v.onStack = true;

		// Consider successors of v
		for(auto e : mE) {
			if(e.a.index != v.index) // Edge doesnt go from v to something else
				continue;
			Vertex& w = e.b;
			
			if(w.index == -1) {
			   // Successor w has not yet been visited; recurse on it
				strongConnect(w, out);
				v.lowLink = std::min(v.lowLink, w.lowLink);
			} else if(w.onStack) {
				// Successor w is in stack S and hence in the current SCC
				// If w is not on stack, then (v, w) is an edge pointing to an SCC already found and must be ignored
				// Note: The next line may look odd - but is correct.
				// It says w.index not w.lowlink; that is deliberate and from the original paper
				v.lowLink = std::min(v.lowLink, w.index);
			}
		}

		// If v is a root node, pop the stack and generate an SCC
		if(v.lowLink == v.index) {
			out.push_back(std::set<Vertex>());
			int w_ind = -1;
			while(w_ind != v.index) {
				Vertex& w = mS.top();
				mS.pop();
				w.onStack = false;
				out.back().insert(w);
			}
		}
	}
};

struct BipartiteGraph {
	// https://www.geeksforgeeks.org/maximum-bipartite-matching/?ref=lbp
	int nLeft, nRight;
	std::map<int, std::vector<int>> linksList;
	std::vector<bool> seenList;
	std::vector<int> matchList;
 
	// Return if a matching is possible for vertex u
	bool matching(int u)
	{		
		std::vector<int> links = linksList.at(u);

		for(int v = 0; v < links.size(); ++v) {
			if(seenList[u])
				continue;
			seenList[u] = true;

			if(matchList[u] == -1 || 
			matching(matchList[u])) {
				matchList[v] = u;
				return true;
			}
		}
		return false;
	}
	void maxMatching()
	{
		std::fill(matchList.begin(), matchList.end(), -1);

		for(int u = 0; u < nLeft; ++u) {
			std::fill(seenList.begin(), seenList.end(), false);
			matching(u);
		}
	}
};

#endif