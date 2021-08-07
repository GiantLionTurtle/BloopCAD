
#ifndef CONSTRAINTSYSTEM_HPP_
#define CONSTRAINTSYSTEM_HPP_

#include "equationsCluster.hpp"
#include "constraint.hpp"

#include <map>
#include <set>

class constraintSystem {
private:
	std::vector<constraint_abstr*> mConstraints;
	std::vector<var_ptr> mVariables;
	// std::map<constraint_abstr*, std::vector<var_ptr>> mConstrToVars;
	// std::map<var_ptr, std::vector<constraint_abstr*>> mVarsToConstr;

	bool mBrokenDown;
	std::vector<equationsCluster*> mSubClusters;

	int mAlgorithm;
	int mNum_activeConstraints;

	int mVerboseLevel;
public:
	constraintSystem(int verboseLevel = 0);
	~constraintSystem();

	bool satisfied();
	void add_constraint(constraint_abstr* constr);
	void add_variable(var_ptr v);
	void add_variables(std::vector<var_ptr> v);
	void toggle_constraint(constraint_abstr* constr, bool enable);

	int solve();
	void breakDown_problem();

	int solver() { return mAlgorithm; }
	void set_solver(int algo) { mAlgorithm = algo; }

	void clear_subClusters();

	var_ptr var(size_t ind) { return mVariables[ind]; }
	void varState(std::vector<VarState>& state);
	void varState(std::map<var_ptr, float>& state);
	void varDelta(std::map<var_ptr, float> first, std::vector<VarDualState>& delta);

	void updatedSystem() { mBrokenDown = false; }

	int num_constraints() { return mNum_activeConstraints; }
	int num_variables() { return mVariables.size(); }
private:
	class constraintGraph {
	public:
		struct flagged_ent {
			int flag;
			int ind;
		};
	private:
		std::vector<flagged_ent> mVert;
		std::map<int, std::vector<int>> mVertToVert;
		int mNumConstr, mNumVar;
	public:
		constraintGraph(std::vector<constraint_abstr*>& constrs, std::vector<var_ptr>& vars);

		int connected_clusters(std::vector<int>& constr_clust, std::vector<int>& var_clust);
	private:	
		void set_clust(int vert, int clust, std::vector<int>& constr_clust, std::vector<int>& var_clust);
	};

};

#endif