
#ifndef CONSTRAINTSYSTEM_HPP_
#define CONSTRAINTSYSTEM_HPP_

#include "EquationsCluster.hpp"
#include "Constraint_abstr.hpp"
#include "DraggedPoint.hpp"

#include <map>
#include <set>


class ConstraintsSystem {
protected:
	std::vector<Constraint_abstr*> mConstraints;
	std::vector<var_ptr> mVariables;
	std::vector<EquationsCluster*> mSubClusters;

	bool mBrokenDown;
	int mSolverType;
	int mNum_liveConstrs, mNum_liveVars;
	int mVerboseLevel;
public:
	ConstraintsSystem(int verboseLevel = 0);
	~ConstraintsSystem();

	bool satisfied();
	void add_constraint(Constraint_abstr* constr);
	void add_variable(var_ptr v);
	void add_variables(std::vector<var_ptr> v);
	void toggle_constraint(Constraint_abstr* constr, bool enable);

	int solve();
	void breakDown_problem();

	int solverType() { return mSolverType; }
	void set_solverType(int type) { mSolverType = type; }

	void clear_subClusters();

	var_ptr var(size_t ind) { return mVariables[ind]; }
	void varState(std::vector<VarState>& state);
	void varState(std::map<var_ptr, float>& state);
	void varDelta(std::map<var_ptr, float> first, std::vector<VarDualState>& delta);

	void updatedSystem() { mBrokenDown = false; }

	int n_constraints() { return mNum_liveConstrs; }
	int n_variables() { return mNum_liveVars; }
private:
	class ConstraintGraph {
	public:
		struct Flagged_node {
			int flag;
			int ind;
		};
	private:
		std::vector<Flagged_node> mVert;
		std::map<int, std::vector<int>> mVertToVert;
		int mNum_Constrs, mNum_Vars;
	public:
		ConstraintGraph(std::vector<Constraint_abstr*>& constrs, std::vector<var_ptr>& vars);

		int connected_clusters(std::vector<int>& constr_clust, std::vector<int>& var_clust);
	private:	
		void set_clust(int vert, int clust, std::vector<int>& constr_clust, std::vector<int>& var_clust);
	};
};

template<size_t nD>
class DragEnabled_ConstraintsSystem : public ConstraintsSystem {
public:
	DragEnabled_ConstraintsSystem(DragSystemHandler* handler, int verboseLevel = 0):
		ConstraintsSystem(verboseLevel)
	{
		mConstraints.push_back(new DraggedPoint<nD>(handler));
	}
};

#endif