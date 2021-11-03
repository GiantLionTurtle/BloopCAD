
#ifndef CONSTRCLUSTER_HPP_
#define CONSTRCLUSTER_HPP_

#include <Utils/Param.hpp>
#include "Constraint_abstr.hpp"

#include <vector>

class ConstrCluster {
private:
	std::vector<Param*> mParams { };
	std::vector<Param*> mOrderedParams { };

	std::vector<Constraint_abstr*> mConstrs { };
	
	int mNValidParams { 0 }, mNSubstitutedParams { 0 };
public:
	ConstrCluster() = default;

	void add_constr(Constraint_abstr* constr);
	void add_param(Param* param);
	void clear();

	void substitute();
	void clear_substitutions();

	void orderParams();
	void updateParams(double* vals);

	// Graph analysis, not live yet
	inline bool couldDecompose() { return false; }
	std::vector<ConstrCluster*> decompose() { return { this }; }
};

#endif