
#include "ConstraintsSystem.hpp"
#include "SolverState.hpp"

#include <Utils/Expunge.hpp>

#define VERBOSE_LEVEL VERBOSE_ALL
#include <Utils/Debug_util.hpp>

ConstraintsSystem::ConstraintsSystem():
	mBrokenDown(false),
	mSolverType(SolverState::LevenbergMarquardt),
	mNum_liveConstrs(0),
	mNum_liveVars(0),
	mNum_liveClusters(0)
{

}
ConstraintsSystem::~ConstraintsSystem()
{
	clear_clusters();
}
bool ConstraintsSystem::satisfied()
{
	for(auto constr : mConstraints) {
		if(!constr->satisfied())
			return false;
	}
	return true;
}
void ConstraintsSystem::add_constraint(Constraint_abstr* constr)
{
	mBrokenDown = false;
	mConstraints.push_back(constr);
}
void ConstraintsSystem::add_variable(var_ptr v)
{
	if(!v)
		return;
	// if(std::find(mVariables.begin(), mVariables.end(), v) != mVariables.end())
		mVariables.push_back(v);
}
void ConstraintsSystem::add_variables(std::vector<var_ptr> vars)
{
	for(auto v : vars)
		add_variable(v);
}

void ConstraintsSystem::toggle_constraint(Constraint_abstr* constr, bool enable)
{
	// if(std::find(mConstraints.begin(), mConstraints.end(), constr) == mConstraints.end())
	// 	return;

	constr->set_exists(enable);
	mBrokenDown = false;
}

int ConstraintsSystem::solve()
{
	clear_substitutions(); // Called here instead of in cluster in the case that substitutions remain in deleted clusters

	verbose(VERBOSE_STEPS, "Start solve attempt...");
	if(!mBrokenDown) {
		verbose(VERBOSE_STEPS, "Breaking down problem...");
		breakDown_problem();
	}

	verbose(VERBOSE_STEPS, "Solving clusters...");
	int output = SolverState::SUCCESS;

	// Try solving the clusters, potentially with a dragged point
	// Note: The number of clusters and the number of live clusters may vary to avoid reallocating later (might change)
	for(int i = 0; i < mNum_liveClusters; ++i) {
		auto clust = mClusters[i];
		verbose(VERBOSE_STEPS, "Solving cluster #"<<i<<"...");
		output = std::max(output, clust->solve());
	}

	// Check if there was an active drag point and remove it (should it only do it if solve fails?)
	bool had_dragged = false;
	for(auto var : mVariables) {
		if(!had_dragged && var->dragged())
			had_dragged = true;
		var->set_dragged(false);
	}

	// Try resolving the cluster without the drag point if it failed with one
	if(output == SolverState::FAILURE && had_dragged) {
		output = SolverState::SUCCESS;
		for(int i = 0; i < mNum_liveClusters; ++i) {
			auto clust = mClusters[i];
			if(clust->output() == SolverState::SUCCESS)
				continue;
			verbose(VERBOSE_STEPS, "Solving cluster #"<<i<<" on second pass...");
			output = std::max(output, clust->solve());
		}
	}

	verbose(VERBOSE_STEPS, "Solve "<<(output == SolverState::SUCCESS ? "success" : "fail"));
	return output;
}

void ConstraintsSystem::breakDown_problem()
{
	std::vector<Constraint_abstr*> liveConstraints;
	std::vector<var_ptr> liveVars;

	// Enforce the exist/dont exist proprety of constraints
	for(auto constr : mConstraints) {
		if(constr->exists())
			liveConstraints.push_back(constr);
	}
	// Enforce the exist/dont exist proprety of variables
	for(auto var : mVariables) {
		if(var->exists())
			liveVars.push_back(var);
	}

	ConstraintGraph g(liveConstraints, liveVars);

	std::vector<int> constr_clust(0), var_clust(0);
	mNum_liveClusters = g.connected_clusters(constr_clust, var_clust);

	// Create new clusters as needed
	for(int i = mClusters.size(); i < mNum_liveClusters; ++i) {
		mClusters.push_back(new EquationsCluster({}, {}, solverType()));
	}

	// It is simpler to clear and readd variables and equation then
	// to try and track change to do chirurgical editing, it might
	// be a future project
	for(int i = 0; i < mNum_liveClusters; ++i) {
		mClusters[i]->set_id(i);
		mClusters[i]->clear();
		mClusters[i]->set_solver(solverType()); //Not useful at the moment but it might be an option to switch solver on the fly
	}

	// Add equations to clusters one at a time
	for (size_t i = 0; i < constr_clust.size(); ++i) {
		int ind = constr_clust[i];
		if(ind < 0 || ind >= mClusters.size()) { // Check if cluster id is valid
			LOG_WARNING("How did we get here??");
			continue; // Should not be reached
		}
		mClusters[ind]->add_constr(liveConstraints[i]);
	}

	// Add variable to clusters one at a time
	for (size_t i = 0; i < var_clust.size(); ++i) {
		int ind = var_clust[i];
		if(ind < 0 || ind >= mClusters.size()) { // Check if cluster id is valid, -1: it is in no cluster (eg. a variable that has no constraint)
			continue;
		}
		mClusters[ind]->add_var(liveVars[i]);
	}

	mBrokenDown = true;
}

void ConstraintsSystem::clear_clusters()
{
	for(int i = 0; i < mClusters.size(); ++i) {
		expunge(mClusters[i]);
	}
	mClusters.clear();
}
void ConstraintsSystem::clear_substitutions()
{
	for(auto var : mVariables) {
		var->clear_substitution();
	}
}

void ConstraintsSystem::varState(std::vector<VarState>& state)
{
	state.resize(mVariables.size());
	for(int i = 0; i < mVariables.size(); ++i) {
		if(mVariables[i]->exists())
			state[i] = VarState(mVariables[i], mVariables[i]->eval());
	}
}
void ConstraintsSystem::varState(std::map<var_ptr, float>& state)
{
	state.clear();
	for(int i = 0; i < mVariables.size(); ++i) {
		if(mVariables[i]->exists())
			state[mVariables[i]] = mVariables[i]->eval();
	}
}
void ConstraintsSystem::varDelta(std::map<var_ptr, float> first, std::vector<VarDualState>& delta)
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

ConstraintsSystem::ConstraintGraph::ConstraintGraph(std::vector<Constraint_abstr*>& constrs, std::vector<var_ptr>& vars):
	mNum_Constrs(0),
	mNum_Vars(0)
{
	std::map<var_ptr, int> v2i; // A variable to index map, the index is the index in the vertex vector 
	
	// Variables AND constraints are in the same graph
	// Constraints are at the begining of the vector and variables after
	for(int i = 0; i < constrs.size() + vars.size(); ++i) {
		if(i < constrs.size()) {
			mVert.push_back({-1, mNum_Constrs}); // This constraint is not assigned to a subgraph (-1)
			mNum_Constrs++;
		} else if(i >= constrs.size()) {
			int vertInd = mNum_Constrs + mNum_Vars;
			v2i[vars[i-constrs.size()]] = vertInd; // Remember where each variable maps onto the vertices array
			mVert.push_back({-1, vertInd}); // This variable is not assigned to a subgraph (-1)
			mNum_Vars++;
		}
	}

	// Create the mappings from variables to constraints and constraint to variables
	for(int c = 0; c < constrs.size(); ++c) {
		for(int v = 0; v < constrs[c]->n_vars(); ++v) {
			var_ptr var = constrs[c]->var(v); // A variable that is part of the constraint
			int vind = v2i[var]; // Retrieve the graph index of the variable
			mVertToVert[c].push_back(vind); // The constraint is linked to the variable
			mVertToVert[vind].push_back(c); // The variable is also linked to the constraint
		}
	}
}
int ConstraintsSystem::ConstraintGraph::connected_clusters(std::vector<int>& constr_clust, std::vector<int>& var_clust)
{
	constr_clust.resize(mNum_Constrs);
	var_clust.resize(mNum_Vars);

	// Fill the constraints' and variables' clusters to invalid (-1)
	for(size_t i = 0; i < constr_clust.size(); ++i) {
		constr_clust[i] = -1;
	}
	for(size_t i = 0; i < var_clust.size(); ++i) {
		var_clust[i] = -1;
	}
	// Set all vertices to invalid cluster (-1)
	for(Flagged_node vert : mVert) {
		vert.flag = -1;
	}
	
	int n_clust = 0; // Number of found clusters
	for(int i = 0; i < mNum_Constrs; ++i) {
		if(mVert[i].flag == -1) // If the flag is not -1, it has a cluster attached to it
			set_clust(i, n_clust++, constr_clust, var_clust); 	// Find all vertices connected and put them into cluster #n_clust
																// then increment the number of found clusters
	}
	return n_clust; // Number of cluster found
}

void ConstraintsSystem::ConstraintGraph::set_clust(int vert, int clust, std::vector<int>& constr_clust, std::vector<int>& var_clust) 
{
	mVert[vert].flag = clust; // This is the vertex's cluster

	if(vert >= mNum_Constrs) {
		var_clust[vert-mNum_Constrs] = clust; // This vertex is a variable
	} else {
		constr_clust[vert] = clust; // This vertex is a constraint
	}

	for(int i : mVertToVert.at(vert)) { // Check all the vertices in the vertex to vertices map
		if(mVert[i].flag == -1) { // If the vertex doesn't have a cluster assigned to it
			set_clust(i, clust, constr_clust, var_clust);
		} else if(mVert[i].flag != clust) {
			LOG_WARNING("How did you get here..");
			break; 	// Should not be reached in a well behaved system because the current vertex 
					// should have been reached by this one that is different
		}
	}
}
