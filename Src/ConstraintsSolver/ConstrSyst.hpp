
#ifndef CONSTRSYST_HPP_
#define CONSTRSYST_HPP_

#include <set>

#include <Utils/Param.hpp>
#include "Constraint_abstr.hpp"
#include "ConstrCluster.hpp"
#include "ConstrGraph.hpp"
#include "SolverState.hpp"

class ConstrSyst {
private:
	std::vector<Param*> mParams;
	std::map<Param*, int> mParam_to_ind;
	std::vector<Constraint_abstr*> mConstrs;
	std::vector<ConstrCluster> mClusters;
	int nActiveClusters { 0 }, nG1, nG2, nG3;

	bool mDecompUpToDate  { false };
public:
	ConstrSyst() = default;

	void add_param(Param* p);
	void add_params(ParamIterator* ps);
	void add_constr(Constraint_abstr* c);

	int solve();

	int create_clusters();
private:
	ConstrGraph create_graph();
	void fix_overConstrained();
};


void ConstrSyst::add_param(Param* p)
{
	mDecompUpToDate  = false;
	if(mParam_to_ind.find(p) == mParam_to_ind.end()) {
		mParam_to_ind[p] = mParams.size();
		mParams.push_back(p);
	}
}
void ConstrSyst::add_params(ParamIterator* ps)
{
	mDecompUpToDate  = false;
	for(int i = 0; i < ps->n_params(); ++i) {
		add_param(ps->param(i));
	}
}
void ConstrSyst::add_constr(Constraint_abstr* c)
{
	mDecompUpToDate  = false;
	mConstrs.push_back(c);
}

int ConstrSyst::solve()
{
	if(!mDecompUpToDate) {
		create_clusters();
	}

	// Good place to do some simple multithreading??
	int out = SolverState::solveOutput::SUCCESS;
	for(auto clust : mClusters) {
		out = std::max(out, clust.solve());
	}
	return out;
}

int ConstrSyst::create_clusters()
{
	ConstrGraph G = create_graph();

	// Clear clusters and resize if needed
	for(int i = 0; i < nActiveClusters && i < mClusters.size(); ++i) {
		mClusters[i].clear();
	}
	if(nActiveClusters > mClusters.size())
		mClusters.resize(nActiveClusters);

	for(int i = 0; i < G.V.size(); ++i) {
		if(G.V[i].metacluster == 1) // Overconstrained
			continue;
		mClusters[G.V[i].cluster].add_param(mParams[G.V[i].data]);
	}
	for(int i = 0; i < G.C.size(); ++i) {
		if(G.C[i].metacluster == 1) // Overconstrained
			continue; 
		mClusters[G.C[i].cluster].add_constr(mConstrs[G.C[i].data]);
	}

	return nG3 > 0 ? SolverState::graphState::OVER_CONSTRAINED : SolverState::graphState::WELL_CONSTRAINED;
}

ConstrGraph ConstrSyst::create_graph()
{
	ConstrGraph G;
	int p = 0;
	for(auto param : mParams) {
		if(!param->exists() || param->frozen() == 2)
			continue;
		G.add_var(p);
		p++;
	}

	int c = 0;
	for(auto constr : mConstrs) {
		if(!constr->exists())
			continue;
		G.add_constr(c);
		for(int j = 0; j < constr->n_params(); ++j) {
			Param* param = constr->param(j);
			if(mParam_to_ind.find(param) == mParam_to_ind.end()) { // This var has never been seen
				continue;
			}
			G.add_c_to_v_ind(c, mParam_to_ind[param]);
		}
		c++; // hehe
	}
	
	std::vector<int> match = G.maxMatching();
	G.make_bidirectionnal(match);
	nActiveClusters = G.mark_Gs(match, nG1, nG2, nG3);

	if(nG2) {
		fix_overConstrained();
	}

	return G;
}

#endif