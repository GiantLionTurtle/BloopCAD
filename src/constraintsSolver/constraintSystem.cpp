
#include "constraintSystem.hpp"

#include <utils/errorLogger.hpp>

constraintSystem::constraintSystem():
	mBrokenDown(false),
	mAlgorithm(algorithm::DogLeg)
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
	mConstraints.push_back(constr);
	for(size_t i = 0; i < constr->n_var(); ++i) {
		variable_ptr var = constr->var(i);
		if(var->is_coef())
			continue;
		mConstrToVars[constr].push_back(var);			
		if(mVarsToConstr.find(var) == mVarsToConstr.end())
			mVariables.push_back(var);
		mVarsToConstr[var].push_back(constr);
	}
}

int constraintSystem::solve()
{
	if(!mBrokenDown) {
		breakDown_problem();
	}
	int output = solveOutput::SUCCESS;
	for(constraintCluster* clust : mSubClusters) {
		switch(mAlgorithm) {
			case algorithm::DogLeg:
				output = std::max(clust->solve_DL(), output);
				break;
			case algorithm::LevenbergMarquardt:
				output = std::max(clust->solve_LM2(), output);
				break;
			default:
				std::cout<<"Unknown solver "<<mAlgorithm<<"\n";
				break;
		}
	}
	return output;
}

void constraintSystem::breakDown_problem()
{
	clear_subClusters();
	constraintGraph g(mConstraints, mVariables);

	std::vector<int> constr_clust(mConstraints.size()), var_clust(mVariables.size());
	int num_clusters = g.connected_clusters(constr_clust, var_clust);
	for(int i = 0; i < num_clusters; ++i) {
		mSubClusters.push_back(new constraintCluster({}, {}, 0));
	}

	for (size_t i = 0; i < constr_clust.size(); ++i) {
		mSubClusters[constr_clust[i]]->mConstraints.push_back(mConstraints[i]);
	}

	for (size_t i = 0; i < var_clust.size(); ++i) {
		mSubClusters[var_clust[i]]->mVariables.push_back(mVariables[i]);
	}
}

void constraintSystem::clear_subClusters()
{
	for(int i = 0; i < mSubClusters.size(); ++i) {
		delete mSubClusters[i];
		mSubClusters[i] = nullptr;
	}
	mSubClusters.clear();
}

std::vector<double> constraintSystem::varState()
{
	std::vector<double> out(mVariables.size());
	for(int i = 0; i < mVariables.size(); ++i) {
		out[i] = mVariables[i]->eval();
	}
	return out;
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


constraintSystem::constraintGraph::constraintGraph(std::vector<std::shared_ptr<constraint_abstract>>& constrs, std::vector<variable_ptr>& vars):
	mNumConstr(constrs.size()),
	mNumVar(vars.size())
{
	std::map<variable_ptr, int> v2i;
	for(int i = 0; i < mNumConstr + mNumVar; ++i) {
		mVert.push_back({-1, i});
		if(i >= mNumConstr) {
			v2i[vars[i-mNumConstr]] = i;
		}
	}
	for(int i = 0; i < mNumConstr; ++i) {
		for(int j = 0; j < constrs[i]->n_var(); ++j) {
			variable_ptr var = constrs[i]->var(j);
			if(var->is_coef())
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
