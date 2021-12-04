
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef CONSTRGRAPH_HPP_
#define CONSTRGRAPH_HPP_

#include "Constraint.hpp"
#include <Utils/Param.hpp>

#include <vector>
#include <stack>
#include <algorithm>
#include <map>
#include <iostream>
#include <set>

struct ConstrGraph {
	struct Vertex {
		int data { -1 };
		int metacluster { 0 }, cluster { -1 };

		Vertex() = default;
	};
	struct Tarjan_helperStruct {
		int index { -1 }, lowLink { -1 };
		bool onStack { false };

		Tarjan_helperStruct() = default;
	};
	std::vector<Vertex> C, V;

	std::map<int, std::vector<int>> C_to_V, V_to_C, C_to_V_inv, V_to_C_inv;

	ConstrGraph() = default;
	ConstrGraph(int nC, int nV)
	{
		C.reserve(nC);
		V.reserve(nV);
	}

	void clear()
	{
		C.clear();
		V.clear();
		C_to_V.clear();
		V_to_C.clear();
		C_to_V_inv.clear();
		V_to_C_inv.clear();
	}

	void add_constr(int c)
	{
		C.push_back({c, 0, -1});
	}
	void add_var(int v)
	{
		V.push_back({v, 0, -1});
	}
	void add_c_to_v_ind(int c, int v)
	{
		C_to_V[c].push_back(v);
		C_to_V_inv[v].push_back(c);
	}
	void add_v_to_c_ind(int v, int c)
	{
		V_to_C[v].push_back(c);
		V_to_C_inv[c].push_back(v);
	}
	int constr(int c)
	{
		if(c < 0 || c >= C.size())
			return -1;
		return C[c].data;
	}
	int var(int v)
	{
		if(v < 0 || v >= V.size())
			return -1;
		return V[v].data;
	}


	int connectedClusters_tarjan(int metacluster, int& cluster)
	{
		int index = 0;

		int cumul_size = C.size() + V.size();
		std::vector<Tarjan_helperStruct> tarjanData(cumul_size);
		std::stack<int> S;

		int n_clusters = cluster;
		for(int v = 0; v < cumul_size; ++v) {
			if(tarjanData[v].index == -1) {
				strongConnect(v, index, metacluster, cluster, tarjanData, S);
			}
		}
		return cluster - n_clusters;
	}

	void strongConnect(	int i, int& index, int metacluster, int& cluster, 
						std::vector<Tarjan_helperStruct>& tarjanData, std::stack<int>& S)
	{
		// If the target node is not in the right metacluster
		if(((i < C.size() && C[i].metacluster != metacluster) || (i >= C.size() && V[i-C.size()].metacluster != metacluster))) {
			return;
		}
		// Set the depth index for v to the smallest unused index
		tarjanData[i].index = index;
		tarjanData[i].lowLink = index;
		index++;
		S.push(i);
		tarjanData[i].onStack = true;

		std::vector<int> connect_list = i >= C.size() ? V_to_C[i-C.size()] : C_to_V[i];
		for(int w : connect_list) {
			int w_lin = -1;
			
			if(i >= C.size()) { // i is a variable, w is a constraint
				w_lin = w;
				if(C[w].metacluster != metacluster)
					continue;
			} else {
				w_lin = w + C.size();
				if(V[w].metacluster != metacluster)
					continue;
			}

			if(tarjanData[w_lin].index == -1) {
				// Successor w has not yet been visited; recurse on it
				strongConnect(w_lin, index, metacluster, cluster, tarjanData, S);
				tarjanData[i].lowLink = std::min(tarjanData[i].lowLink, tarjanData[w_lin].lowLink);
			} else if (tarjanData[w_lin].onStack) {
				// Successor w is in stack S and hence in the current SCC
				// If w is not on stack, then (v, w) is an edge pointing to an SCC already found and must be ignored
				// Note: The next line may look odd - but is correct.
				// It says w.index not w.lowlink; that is deliberate and from the original paper
				tarjanData[i].lowLink = std::min(tarjanData[i].lowLink, tarjanData[w_lin].index);
			}
		}

		// If v is a root node, pop the stack and generate an SCC
		if(tarjanData[i].lowLink == tarjanData[i].index) {
			int w_ind = -1;
			while(w_ind != tarjanData[i].index) {
				int w = S.top();
				S.pop();
				tarjanData[w].onStack = false;
				if(w >= C.size()) {
					V[w-C.size()].cluster = cluster;
				} else {
					C[w].cluster = cluster;
				}
				w_ind = tarjanData[w].index;
			}
			cluster++;
		}
	}

	// Return if a matching is possible for vertex u
	bool matching(int c, std::vector<bool>& seenList, std::vector<int>& matchList)
	{
		auto links = C_to_V.find(c);
		if(links == C_to_V.end())
			return false;

		for(int v : links->second) {
			if(seenList[v])
				continue;
			seenList[v] = true;

			if(matchList[v] == -1 || 
			matching(matchList[v], seenList, matchList)) {
				matchList[v] = c;
				return true;
			}
		}
		return false;
	}
	std::vector<int> maxMatching()
	{
		std::vector<bool> seenList(V.size());
		std::vector<int> matchList(V.size());
		std::fill(matchList.begin(), matchList.end(), -1);

		for(int c = 0; c < C.size(); ++c) {
			std::fill(seenList.begin(), seenList.end(), false);
			matching(c, seenList, matchList);
		}
		return matchList;
	}

	void make_bidirectionnal(std::vector<int> const& matching)
	{
		for(int i = 0; i < matching.size(); ++i) {
			if(matching[i] == -1)
				continue;
			add_v_to_c_ind(i, matching[i]);
		}
	}

	void mark_ancestors_v(int v, int metacluster, int cluster)
	{
		if(C_to_V_inv.find(v) == C_to_V_inv.end())
			return;
		for(int i = 0; i < C_to_V_inv[v].size(); ++i) {
			int ancestor_ind = C_to_V_inv[v][i];
			if(C[ancestor_ind].metacluster != metacluster) {
				C[ancestor_ind].metacluster = metacluster;
				C[ancestor_ind].cluster = cluster;
				mark_ancestors_c(ancestor_ind, metacluster, cluster);
			}
		}
	}
	void mark_ancestors_c(int c, int metacluster, int cluster)
	{
		if(V_to_C_inv.find(c) == V_to_C_inv.end())
			return;
		for(int i = 0; i < V_to_C_inv[c].size(); ++i) {
			int ancestor_ind = V_to_C_inv[c][i];
			if(V[ancestor_ind].metacluster != metacluster) {
				V[ancestor_ind].metacluster = metacluster;
				V[ancestor_ind].cluster = cluster;
				mark_ancestors_v(ancestor_ind, metacluster, cluster);
			}
		}
	}
	void mark_descendant_c(int c, int metacluster)
	{
		if(C_to_V.find(c) == C_to_V.end())
			return;
		for(int i = 0; i < C_to_V[c].size(); ++i) {
			int descendant_ind = C_to_V[c][i];
			if(V[descendant_ind].metacluster != metacluster) {
				V[descendant_ind].metacluster = metacluster;
				mark_descendant_v(descendant_ind, metacluster);
			}
		}
	}
	void mark_descendant_v(int v, int metacluster)
	{
		if(V_to_C.find(v) == V_to_C.end())
			return;
		for(int i = 0; i < V_to_C[v].size(); ++i) {
			int descendant_ind = V_to_C[v][i];
			if(C[descendant_ind].metacluster != metacluster) {
				C[descendant_ind].metacluster = metacluster;
				mark_descendant_c(descendant_ind, metacluster);
			}
		}		
	}

	int mark_Gs(std::vector<int> const& matching, int& nG1, int& nG2, int& nG3)
	{
		int clusterID = 0;
		std::vector<bool> saturated_C(C.size(), false);
		for(int i = 0; i < matching.size(); ++i) {
			if(matching[i] == -1) {
				V[i].metacluster = 2;
				// Only one G3 cluster, so it's always cluster 0
				V[i].cluster = 0;
				mark_ancestors_v(i, 2, 0); 
				clusterID = 1;
				nG3 = 1;
				continue;
			}
			saturated_C[matching[i]] = true;
		}
		for(int i = 0; i < saturated_C.size(); ++i) {
			if(saturated_C[i])
				continue;
			C[i].metacluster = 1;
			mark_descendant_c(i, 1);
		}

		nG1 = connectedClusters_tarjan(0, clusterID);
		nG2 = connectedClusters_tarjan(1, clusterID);
		// nG3 = connectedClusters_tarjan(2, clusterID);
		return clusterID;
	}
};

#endif