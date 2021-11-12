
#ifndef CONSTRSYST_HPP_
#define CONSTRSYST_HPP_

#include <set>

#include <Utils/Param.hpp>
#include "Constraint_abstr.hpp"
#include "ConstrCluster.hpp"
#include "ConstrGraph.hpp"
#include "SolverState.hpp"

class ConstrSyst {
private:
	std::vector<Param*> mParams;
	std::map<Param*, int> mParam_to_ind;
	std::vector<Constraint_abstr*> mConstrs;
	std::vector<ConstrCluster> mClusters;
	int nActiveClusters { 0 }, nG1, nG2, nG3;
	int mAlgorithm { 0 };

	bool mDecompUpToDate  { false };
public:
	ConstrSyst() = default;
	ConstrSyst(int algo);

	void add_param(Param* p);
	void add_params(ParamIterator* ps);
	void add_constr(Constraint_abstr* c);

	inline void force_decomposition() { mDecompUpToDate = false; }

	inline std::vector<Param*>& params() { return mParams; }

	int solve();

	int create_clusters();
private:
	ConstrGraph create_graph();
	void fix_overConstrained() {}
};

#endif