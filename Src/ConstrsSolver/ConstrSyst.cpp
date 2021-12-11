
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
	if(c->substitutionConstraint()) {
		mConstrsSubst.push_back(c);
	} else {
		mDecompUpToDate  = false;
		mConstrsEval.push_back(c);
	}

	c->set_syst(this);
    add_params(c);
}

int ConstrSyst::solve()
{
	if(!mDecompUpToDate) {
		create_clusters();
	}
	clear_drag();

	substitute();

	// Good place to do some simple multithreading??
	int out = SolverState::solveOutput::SUCCESS;
	for(int i = 0; i < nActiveClusters; ++i) {
		out = std::max(out, mClusters[i].solve_numeric());
	}

	clear_substitutions();
	clear_drag();

	if(out == SolverState::solveOutput::SUCCESS)
		return out;

	std::cout<<"Solving a second time.\n";
	
	out = SolverState::solveOutput::SUCCESS;
	substitute();
	for(int i = 0; i < nActiveClusters; ++i) {
		if(mClusters[i].output() == SolverState::solveOutput::SUCCESS)
			continue;
		out = std::max(out, mClusters[i].solve_numeric());
	}

	clear_substitutions();
	return out;
}

void ConstrSyst::substitute()
{
	for(auto b : mSubstBlobs) {
		b->choose_driving();
	}
}
void ConstrSyst::clear_substitutions()
{
	for(auto b : mSubstBlobs) {
		b->update_params();
	}
}
void ConstrSyst::clear_drag()
{
	for(auto p : mParams) {
		p->set_frozen(Param::Frozen_levels::UNFROZEN);
	}
}

int ConstrSyst::create_clusters()
{
	mDecompUpToDate = true;
	create_substBlobs();
	if(!create_graph())
		return SolverState::graphState::INVALID_GRAPH;

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
		Param* p;
		int ind = mG.V[i].data;
		if(ind < mSubstBlobs.size()) {
			p = mSubstBlobs[ind];
		} else {
			p = mParams[ind - mSubstBlobs.size()];
		}
		mClusters[mG.V[i].cluster].add_param(p);
	}
	for(int i = 0; i < mG.C.size(); ++i) {
		if(mG.C[i].metacluster == 1) // Overconstrained
			continue; 
		mClusters[mG.C[i].cluster].add_constr(mConstrsEval[mG.C[i].data]);
	}

	for(int i = 0; i < mClusters.size(); ++i) {
		mClusters[i].configure();
	}

	return nG3 > 0 ? SolverState::graphState::OVER_CONSTRAINED : SolverState::graphState::WELL_CONSTRAINED;
}
void ConstrSyst::create_substBlobs()
{
	for(auto b : mSubstBlobs) {
		b->release();
		b->clear();
	}
	mSubstBlobs.clear();
	
	for(auto c : mConstrsSubst) {
		if(c->exists())
			c->append_substBlobs(mSubstBlobs);
	}
}
bool ConstrSyst::create_graph()
{
	
	mG.clear();
	int p = 0;
	std::map<Param*, int> param_to_ind;
	std::map<SubstBlob*, int> blob_to_ind;
	std::set<SubstBlob*> blobsAdded;

	for(int i = 0; i < mSubstBlobs.size(); ++i) {
		mG.add_var(i);
		blob_to_ind[mSubstBlobs[i]] = p;
		p++;
	}
	for(int i = 0; i < mParams.size(); ++i) {
		auto param = mParams[i];
		if(!param->exists())
			continue;
		if(param->blob() != nullptr)
			continue;
		mG.add_var(i+mSubstBlobs.size());
		param_to_ind[param] = p;
		p++;
	}
	if(p == 0) // No variable in the graph
		return false;

	int c = 0;
	for(auto constr : mConstrsEval) {
		if(!constr->exists())
			continue;
		mG.add_constr(c);
		for(int j = 0; j < constr->n_params(); ++j) {
			Param* param = constr->param(j);
			int v_ind = -1;
			if(param->blob()) {
				if(blobsAdded.find(param->blob()) != blobsAdded.end())
					continue;
				blobsAdded.insert(param->blob());
				auto find = blob_to_ind.find(param->blob());
				if(find == blob_to_ind.end())
					continue;
				v_ind = find->second;
			} else {
				auto find = param_to_ind.find(param);
				if(find == param_to_ind.end())
					continue;
				v_ind = find->second;
			}
			mG.add_c_to_v_ind(c, v_ind);
		}
		c++; // hehe
	}
	if(c == 0) // No Constraint in the graph
		return false;

	std::vector<int> match = mG.maxMatching();
	mG.make_bidirectionnal(match);
	nActiveClusters = mG.mark_Gs(match, nG1, nG2, nG3);

	if(nG2) {
		fix_overConstrained();
	}
	return true;
}
