
// BloopCAD
// Copyright (C) 2020  BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef CONSTRSYST_HPP_
#define CONSTRSYST_HPP_

#include <set>

#include <Utils/Param.hpp>
#include "Constraint.hpp"
#include "ConstrCluster.hpp"
#include "ConstrGraph.hpp"
#include "SolverState.hpp"

class ConstrSyst {
private:
	std::vector<Param*> mParams;
	std::map<Param*, int> mParam_to_ind;
	std::vector<Constraint*> mConstrsEval { }, mConstrsSubst { };
	std::vector<ConstrCluster> mClusters;
	ConstrGraph mG;
	int nActiveClusters { 0 }, nG1 { 0 }, nG2 { 0 }, nG3 { 0 };
	int mAlgorithm { 0 };

	bool mDecompUpToDate  { false };
public:
	ConstrSyst() = default;
	ConstrSyst(int algo);

	void add_param(Param* p);
	void add_params(ParamIterator* ps);
	void add_constr(Constraint* c);

	inline void force_decomposition() { mDecompUpToDate = false; }

	inline std::vector<Param*>& params() { return mParams; }

	int solve();

	void substitute();
	void clear_substitutions();
	void clear_drag();

	int create_clusters();
private:
	bool create_graph();
	void fix_overConstrained() {}
};

#endif