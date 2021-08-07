
#include "constraintSystem.hpp"
#include "solverState.hpp"

#include <utils/DebugUtils.hpp>

constraintSystem::constraintSystem(int verboseLevel):
	mBrokenDown(false),
	mAlgorithm(solverState::DogLeg),
	mVerboseLevel(verboseLevel),
	mNum_activeConstraints(0)
{

}
constraintSystem::~constraintSystem()
{
	clear_subClusters();
}
bool constraintSystem::satisfied()
{
	for(auto constr : mConstraints) {
		if(!constr->satisfied())
			return false;
	}
	return true;
}
void constraintSystem::add_constraint(constraint_abstr* constr) 
{
	mNum_activeConstraints++;
	mBrokenDown = false;
	mConstraints.push_back(constr);
	// for(size_t i = 0; i < constr->n_vars(); ++i) {
	// 	var_ptr var = constr->var(i);
	// 	if(var->is_coef())
	// 		continue;
	// 	mConstrToVars[constr].push_back(var);			
	// 	if(mVarsToConstr.find(var) == mVarsToConstr.end())
	// 		mVariables.push_back(var);
	// 	mVarsToConstr[var].push_back(constr);
	// }
}
void constraintSystem::add_variable(var_ptr v)
{
	if(!v)
		return;
	// if(std::find(mVariables.begin(), mVariables.end(), v) != mVariables.end())
		mVariables.push_back(v);
}
void constraintSystem::add_variables(std::vector<var_ptr> vars)
{
	for(auto v : vars)
		add_variable(v);
}

void constraintSystem::toggle_constraint(constraint_abstr* constr, bool enable)
{
	if(std::find(mConstraints.begin(), mConstraints.end(), constr) == mConstraints.end())
		return;

	mNum_activeConstraints += enable ? 1 : -1;
	constr->set_exists(enable);
	mBrokenDown = false;
}

int constraintSystem::solve()
{
	if(mVerboseLevel) 
		std::cout<<"Start solve attempt...\n";
	if(!mBrokenDown) {
		if(mVerboseLevel) 
			std::cout<<"Breaking down problem...\n";
		breakDown_problem();
	}

	if(mVerboseLevel)
		std::cout<<"Solving clusters...";
	int output = solverState::SUCCESS;
	for(auto clust : mSubClusters) {
		output = std::max(output, clust->solve());
	}
	if(mVerboseLevel) 
		std::cout<<"Solve "<<(output == solverState::SUCCESS ? "success" : "fail")<<".\n";

	return output;
}

void constraintSystem::breakDown_problem()
{
	mBrokenDown = false;

	std::vector<constraint_abstr*> liveConstraints;
	std::vector<var_ptr> liveVars;

	for(auto constr : mConstraints) {
		if(constr->exists())
			liveConstraints.push_back(constr);
	}
	for(auto var : mVariables) {
		if(var->exists())
			liveVars.push_back(var);
	}

	clear_subClusters();
	constraintGraph g(liveConstraints, liveVars);

	std::vector<int> constr_clust(0), var_clust(0);
	int num_clusters = g.connected_clusters(constr_clust, var_clust);
	for(int i = 0; i < num_clusters; ++i) {
		mSubClusters.push_back(new equationsCluster({}, {}, mAlgorithm, 1));
		mSubClusters.back()->set_id(i);
	}

	for (size_t i = 0; i < constr_clust.size(); ++i) {
		int ind = constr_clust[i];
		if(ind >= mSubClusters.size())
			continue;
		for(size_t j = 0; j < liveConstraints[i]->n_equs(); ++j) {
			mSubClusters[ind]->add_equ(liveConstraints[i]->equ(j));
		}
	}

	for (size_t i = 0; i < var_clust.size(); ++i) {
		int ind = var_clust[i];
		if(ind >= mSubClusters.size())
			continue;
		mSubClusters[ind]->add_var(liveVars[i]);
	}

	for(size_t i = 0; i < mSubClusters.size(); ++i) {
		mSubClusters[i]->init();
	}

	mBrokenDown = true;
}

void constraintSystem::clear_subClusters()
{
	for(int i = 0; i < mSubClusters.size(); ++i) {
		mSubClusters[i]->clear_substitutions();
		mSubClusters[i]->clear_tags();
		delete mSubClusters[i];
		mSubClusters[i] = nullptr;
	}
	mSubClusters.clear();
}

void constraintSystem::varState(std::vector<VarState>& state)
{
	state.resize(mVariables.size());
	for(int i = 0; i < mVariables.size(); ++i) {
		if(mVariables[i]->exists())
			state[i] = VarState(mVariables[i], mVariables[i]->eval());
	}
}
void constraintSystem::varState(std::map<var_ptr, float>& state)
{
	state.clear();
	for(int i = 0; i < mVariables.size(); ++i) {
		if(mVariables[i]->exists())
			state[mVariables[i]] = mVariables[i]->eval();
	}
}
void constraintSystem::varDelta(std::map<var_ptr, float> first, std::vector<VarDualState>& delta)
{
	delta.clear();
	for(int i = 0; i < mVariables.size(); ++i) {
		if(mVariables[i]->exists() && first.find(mVariables[i]) != first.end()) {
			float diff = mVariables[i]->eval() - first[mVariables[i]];
			if(diff != 0.0f)
				delta.push_back(VarDualState(mVariables[i], mVariables[i]->eval(), first[mVariables[i]]));
		}
	}
}

constraintSystem::constraintGraph::constraintGraph(std::vector<constraint_abstr*>& constrs, std::vector<var_ptr>& vars):
	mNumConstr(0),
	mNumVar(0)
{
	std::map<var_ptr, int> v2i;
	for(int i = 0; i < constrs.size() + vars.size(); ++i) {
		if(i < constrs.size() && constrs[i]->exists()) {
			mVert.push_back({-1, i});
			mNumConstr++;
		} else if(i >= constrs.size() && vars[i-constrs.size()]->exists()) {
			v2i[vars[i-constrs.size()]] = i;
			mVert.push_back({-1, i});
			mNumVar++;
		}
	}
	for(int i = 0; i < constrs.size(); ++i) {
		if(!constrs[i]->exists())
			continue;
		for(int j = 0; j < constrs[i]->n_vars(); ++j) {
			var_ptr var = constrs[i]->var(j);
			if(var->is_coef() || !var->exists())
				continue;
			int vind = v2i[var];
			mVertToVert[i].push_back(vind);
			mVertToVert[vind].push_back(i);
		}
	}
}
int constraintSystem::constraintGraph::connected_clusters(std::vector<int>& constr_clust, std::vector<int>& var_clust)
{
	constr_clust.resize(mNumConstr);
	var_clust.resize(mNumVar);
	for(flagged_ent vert : mVert) {
		vert.flag = -1;
	}

	int num_clust = 0;
	for(int i = 0; i < mNumConstr; ++i) {
		if(mVert[i].flag == -1) {
			set_clust(i, num_clust++, constr_clust, var_clust);
		}
	}
	return num_clust;
}

void constraintSystem::constraintGraph::set_clust(int vert, int clust, std::vector<int>& constr_clust, std::vector<int>& var_clust) 
{
	mVert[vert].flag = clust;

	if(vert >= mNumConstr) {
		var_clust[vert-mNumConstr] = clust;
	} else {
		constr_clust[vert] = clust;
	}

	for(int i : mVertToVert.at(vert)) {
		if(mVert[i].flag == -1) {
			set_clust(i, clust, constr_clust, var_clust);
		} else if(mVert[i].flag != clust) {
			std::cout<<"Reeee\n";
			break;
		}
	}
}
