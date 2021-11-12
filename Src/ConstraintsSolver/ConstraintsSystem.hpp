
#ifndef CONSTRAINTSYSTEM_HPP_
#define CONSTRAINTSYSTEM_HPP_

#include "EquationsCluster.hpp"
#include "Constraint_abstr.hpp"
#include "Expression.hpp"

#include <Utils/Param.hpp>

#include <map>
#include <set>

/*
	@class ConstraintsSystem manages constraints and variables and ensures
	that all constraints are satisfied (if possible)
*/
class ConstraintsSystem {
protected:
	std::vector<Constraint_abstr*> mConstraints; 	// All constraints that are handled by the system
	std::vector<Param*> mVariables;				// All variables that are handled by the system
	std::vector<EquationsCluster*> mClusters;		// Clusters of equations and variables used internally to solve the system

	bool mBrokenDown;	// If there are new variables/constraints that must be taken into account
	int mSolverType;	// Name of the numerical solver in use
	int mNum_liveConstrs, mNum_liveVars; // Number of variables&constraints that *exist*
	int mNum_liveClusters; 	// Number of clusters that are used to solve the system, 
							// the clusters used are garanteed to be at the begining of the vector
public:
	/*
		@constructor ConstraintsSystem creates a system with no variables and no constraints
		they are added later on
	*/
	ConstraintsSystem();
	~ConstraintsSystem();

	/*
		@function satisfied checks if all constraints are satisfied

		@return If all constraints are satisfied
	*/
	bool satisfied();
	/*
		@function add_constraint adds a constraint to be managed and sets the brokenUp flag to false

		@param constr The constraint to add to the system
	*/
	void add_constraint(Constraint_abstr* constr);
	/*
		@function add_variable adds a variable to be managed and sets the brokenUp flag to false

		@param v The variable to add to the system
	*/	
	void add_variable(Param* v);
	/*
		@function add_variables adds multiple variables to be managed and sets the brokenUp flag to false

		@param v The variables to add to the system
	*/		
	void add_variables(ParamIterator* it);

	/*
		@function toggle_constraint sets a constraint's existence to a specific state and set the brokenUp flag to false

		@param constr The constraint to be toggled
		@param enable The *existence* state that must be set
	*/
	void toggle_constraint(Constraint_abstr* constr, bool enable);

	/*
		@function solve solves the system cluster by cluster

		@return The output flag (defined in the SolverState enum)
		0: RUNNING, 1: SUCCESS, 2: FAILURE, 3: INVALID
	*/
	int solve();
	/*
		@function breakDown_problem breaks the system into clusters

		@note If the system is trully tangled, it might make a single cluster
	*/
	void breakDown_problem();

	/*
		@function solverType

		@return The numerical solver name (defined in the SolverState enum)
		0: None, 1: DogLeg, 2: LevenbergMarquardt 
	*/
	int solverType() { return mSolverType; }
	/*
		@function set_solverType sets the current numerical solver

		@param type The numerical solver name (defined in the SolverState enum)
		0: None, 1: DogLeg, 2: LevenbergMarquardt
	*/
	void set_solverType(int type) { mSolverType = type; }

	/*
		@function clear_clusters destroys the clusters
	*/
	void clear_clusters();
	/*
		@function clear_substitutions removes substitution of each variable

		@note It is often called after @function apply_substitutions (or in the next loop cycle)
	*/
	void clear_substitutions();

	/*
		@function var is a getter for the variables of the system

		@param ind The index of the requested variable
		@return The requested variable if the index is not out of bounds
	*/
	Param* var(size_t ind) { return mVariables[ind]; }
	/*
		@function varState pulls the variables states in a linear fashion

		@param state The vector into which to drop the variables states
		@note A variable state is a tupple struct of a Param* and a value [float]
	*/
	void varState(std::vector<ParamState>& state);
	/*
		@function varState pulls the variables states in a mapped fashion

		@param state The map into which to drop the variables states
		@note A variable state is a tupple struct of a Param* and a value [float]
	*/
	void varState(std::map<Param*, float>& state);
	/*
		@function varDelta pulls combines variables states and variables current state in a linear fashion

		@param first The initial states of the variables in a mapped fashion
		@param delta The vector into which to drop the variables states
		@note A variable dual state is a tupple struct of a Param* and two values [float] (presumably before and after an event)
	*/
	void varDelta(std::map<Param*, float> first, std::vector<ParamDualState>& delta);

	/*
		@function updatedSystem sets the brokenDown flag to false, it is used to notify the system of 
		a change or to force a cluster breakdown on next solve
	*/
	void updatedSystem() { mBrokenDown = false; }

	/*
		@function n_constraints gives the number of constraints that *exist*

		@return The number of *existing* constraints
	*/
	int n_constraints() { return mNum_liveConstrs; }
	/*
		@function n_variables gives the number of variables that *exist*

		@return The number of *existing* variable
	*/
	int n_variables() { return mNum_liveVars; }
private:
	/*
		@class ConstraintGraph is an helper class to breakdown the system
	*/
	class ConstraintGraph {
	public:
		/*
			@struct Flagged_node is an helper struct to manage the ConstraintGraph
		*/
		struct Flagged_node {
			int flag; // Subgraph index ; -1 if it is not assigned yet
			int ind; // Index in the vertices vector	
		};
	private:
		std::vector<Flagged_node> mVert;
		std::map<int, std::vector<int>> mVertToVert; 	// How each vertex is connected to other vertices
														// the key is the index of the vertex and it maps
														// to a list of indices of vertices
		int mNum_Constrs, mNum_Vars; // The graph is split in half, remember were variables and were constraints begin in the array
	public:
		/*
			@constructor ConstraintGraph creates the association map
		*/
		ConstraintGraph(std::vector<Constraint_abstr*>& constrs, std::vector<Param*>& vars);

		/*
			@function connected_clusters sorts vertices (constraints and variables into clusters and give a cluster ID
			to every constraint and every cluster)

			@param constr_clust [out]	The cluster id of every constraint (in the order they were put in at creation)
			@param var_clust [out] 		The cluster id of every variable (in the order they were put in at creation)

			@return The number of found cluster
		*/
		int connected_clusters(std::vector<int>& constr_clust, std::vector<int>& var_clust);
	private:
		/*
			@function set_clust sets the cluster id of a vertex recursively (to its adjacent vertices)

			@param vert 		The vertex that must receive a cluster id
			@param clust 		The id of the cluster
			@param constr_clust The current list of cluster ids of constraint vertices (-1 = no cluster)
			@param constr_clust The current list of cluster ids of variable vertices (-1 = no cluster)
		*/
		void set_clust(int vert, int clust, std::vector<int>& constr_clust, std::vector<int>& var_clust);
	};
};

#endif