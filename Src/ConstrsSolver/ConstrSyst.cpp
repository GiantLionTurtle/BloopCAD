
// BloopCAD
// Copyright (C) 2020  BloopCorp

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

#include "ConstrSyst.hpp"

ConstrSyst::ConstrSyst(int algo)
	: mAlgorithm(algo)
{

}

void ConstrSyst::add_param(Param* p)
{
	if(mParam_to_ind.find(p) == mParam_to_ind.end()) {
        mDecompUpToDate  = false; // If the parameter is known, no need for decomposition
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
void ConstrSyst::add_constr(Constraint* c)
{
	mDecompUpToDate  = false;
	mConstrs.push_back(c);
	c->set_syst(this);
    add_params(c);
}

int ConstrSyst::solve()
{
	if(!mDecompUpToDate) {
		create_clusters();
	}

	// Good place to do some simple multithreading??
	int out = SolverState::solveOutput::SUCCESS;
	for(int i = 0; i < nActiveClusters; ++i) {
		out = std::max(out, mClusters[i].solve());
	}
	return out;
}

int ConstrSyst::create_clusters()
{
	mDecompUpToDate = true;
	create_graph();

	// Clear clusters and resize if needed
	for(int i = 0; i < nActiveClusters && i < mClusters.size(); ++i) {
		mClusters[i].clear();
	}
	for(int i = mClusters.size(); i < nActiveClusters; ++i) {
		mClusters.push_back(ConstrCluster(mAlgorithm));
	}
	

	for(int i = 0; i < mG.V.size(); ++i) {
		if(mG.V[i].metacluster == 1) // Overconstrained
			continue;
		mClusters[mG.V[i].cluster].add_param(mParams[mG.V[i].data]);
	}
	for(int i = 0; i < mG.C.size(); ++i) {
		if(mG.C[i].metacluster == 1) // Overconstrained
			continue; 
		mClusters[mG.C[i].cluster].add_constr(mConstrs[mG.C[i].data]);
	}

	return nG3 > 0 ? SolverState::graphState::OVER_CONSTRAINED : SolverState::graphState::WELL_CONSTRAINED;
}

void ConstrSyst::create_graph()
{
	mG.clear();
	int p = 0;
	for(auto param : mParams) {
		if(!param->exists() || param->frozen() == 2)
			continue;
		mG.add_var(p);
		p++;
	}

	int c = 0;
	for(auto constr : mConstrs) {
		if(!constr->exists())
			continue;
		mG.add_constr(c);
		for(int j = 0; j < constr->n_params(); ++j) {
			Param* param = constr->param(j);
			if(mParam_to_ind.find(param) == mParam_to_ind.end()) { // This var has never been seen
				continue;
			}
			mG.add_c_to_v_ind(c, mParam_to_ind[param]);
		}
		c++; // hehe
	}
	
	std::vector<int> match = mG.maxMatching();
	mG.make_bidirectionnal(match);
	nActiveClusters = mG.mark_Gs(match, nG1, nG2, nG3);

	if(nG2) {
		fix_overConstrained();
	}
}
