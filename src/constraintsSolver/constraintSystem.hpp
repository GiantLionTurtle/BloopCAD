
#ifndef CONSTRAINTSYSTEM_HPP_
#define CONSTRAINTSYSTEM_HPP_

#include "constraintCluster.hpp"
#include "constraint.hpp"

#include <map>

class constraintSystem {
public:
	enum algorithm { DogLeg, LevenbergMarquardt };
private:
	std::vector<std::shared_ptr<constraint_abstract>> mConstraints;
	std::vector<var_ptr> mVariables;
	std::map<std::shared_ptr<constraint_abstract>, std::vector<var_ptr>> mConstrToVars;
	std::map<var_ptr, std::vector<std::shared_ptr<constraint_abstract>>> mVarsToConstr;

	bool mBrokenDown;
	std::vector<constraintCluster*> mSubClusters;

	int mAlgorithm;

	int mVerboseLevel;
public:
	constraintSystem(int verboseLevel = 0);
	~constraintSystem();

	bool satisfied();
	void add_constraint(std::shared_ptr<constraint_abstract> constr);

	int solve();
	void breakDown_problem();

	int solver() { return mAlgorithm; }
	void set_solver(int algo) { mAlgorithm = algo; }

	void clear_subClusters();

	void varState(std::vector<double>& state);
	void set_varState(std::vector<double> state);

	void updatedSystem() { mBrokenDown = false; }

	int num_constraints() { return mConstraints.size(); }
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
		constraintGraph(std::vector<std::shared_ptr<constraint_abstract>>& constrs, std::vector<var_ptr>& vars);

		int connected_clusters(std::vector<int>& constr_clust, std::vector<int>& var_clust);
	private:	
		void set_clust(int vert, int clust, std::vector<int>& constr_clust, std::vector<int>& var_clust);
	};

};

#endif