
#include "constraintSystem.hpp"

#include <utils/errorLogger.hpp>

constraintSystem::constraintSystem(int verboseLevel):
	mBrokenDown(false),
	mAlgorithm(algorithm::DogLeg),
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
	for(std::shared_ptr<constraint_abstract> constr : mConstraints) {
		if(!constr->satisfied())
			return false;
	}
	return true;
}
void constraintSystem::add_constraint(std::shared_ptr<constraint_abstract> constr) 
{
	mNum_activeConstraints++;
	mBrokenDown = false;
	mConstraints.push_back(constr);
	for(size_t i = 0; i < constr->n_vars(); ++i) {
		var_ptr var = constr->var(i);
		if(var->is_coef())
			continue;
		mConstrToVars[constr].push_back(var);			
		if(mVarsToConstr.find(var) == mVarsToConstr.end())
			mVariables.push_back(var);
		mVarsToConstr[var].push_back(constr);
	}
}
void constraintSystem::toggle_constraint(std::shared_ptr<constraint_abstract> constr, bool enable)
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
	int output = constraintCluster::SUCCESS;
	for(constraintCluster* clust : mSubClusters) {
		output = std::max(output, clust->solve());
	}
	if(mVerboseLevel) 
		std::cout<<"Solve "<<(output == constraintCluster::SUCCESS ? "success" : "fail")<<".\n";

	return output;
}

void constraintSystem::breakDown_problem()
{
	mBrokenDown = false;
	clear_subClusters();
	constraintGraph g(mConstraints, mVariables);

	std::vector<int> constr_clust(0), var_clust(0);
	int num_clusters = g.connected_clusters(constr_clust, var_clust);
	for(int i = 0; i < num_clusters; ++i) {
		mSubClusters.push_back(new constraintCluster({}, {}, mAlgorithm, 1));
		mSubClusters.back()->set_id(i);
	}

	for (size_t i = 0; i < constr_clust.size(); ++i) {
		int ind = constr_clust[i];
		if(ind >= mSubClusters.size())
			continue;
		mSubClusters[ind]->mConstraints.push_back(mConstraints[i]);
	}

	for (size_t i = 0; i < var_clust.size(); ++i) {
		int ind = var_clust[i];
		if(ind >= mSubClusters.size())
			continue;
		mSubClusters[ind]->mVariables.push_back(mVariables[i]);
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

void constraintSystem::varState(std::vector<double>& state)
{
	state.resize(mVariables.size());
	for(int i = 0; i < mVariables.size(); ++i) {
		state[i] = mVariables[i]->eval();
	}
}
void constraintSystem::set_varState(std::vector<double> state)
{
	if(state.size() != mVariables.size()) {
		LOG_WARNING("Given state does not match current size.");
		// return;
	}
	for(int i = 0; i < mVariables.size() && i < state.size(); ++i) {
		mVariables[i]->set(state[i]);
	}
}

constraintSystem::constraintGraph::constraintGraph(std::vector<std::shared_ptr<constraint_abstract>>& constrs, std::vector<var_ptr>& vars):
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
