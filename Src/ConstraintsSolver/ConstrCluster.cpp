
#include "ConstrCluster.hpp"

void ConstrCluster::add_constr(Constraint_abstr* constr)
{
	mConstrs.push_back(constr);
}
void ConstrCluster::add_param(Param* param)
{
	mParams.push_back(param);
}

void ConstrCluster::clear()
{
	mParams.clear();
	mOrderedParams.clear();
	mConstrs.clear();
}

void ConstrCluster::substitute()
{
	for(auto constr : mConstrs) {
		constr->substitute();
	}
}
void ConstrCluster::clear_substitutions()
{
	for(int i = mOrderedParams.size(); i > mOrderedParams.size() - mNSubstitutedParams; --i) {
		mOrderedParams[i]->delete_substitution();
	}
}

void ConstrCluster::orderParams()
{
	mNValidParams = 0;
	mNSubstitutedParams = 0;
	int nParams = mParams.size();

	for(int i = 0; i < nParams; ++i) {
		if(mParams[i]->substituted()) {
			mOrderedParams[nParams - (mNSubstitutedParams++)];
		} else if(!mParams[i]->frozen()) {
			mOrderedParams[mNValidParams++];
		}
	}
	// There might be a hole in the middle of the array for frozen parameters
	// Anyway those don't matter for the solve
}

void ConstrCluster::updateParams(double* vals)
{
	// Don't know if all this unrolling is that necessary, just found out about
	// Duff's device and wanted to try 
	// https://en.wikipedia.org/wiki/Duff's_device

	// Set the values of the valid parameters (not dragged nor substituted parameters)
	{
		int n = (mNValidParams+7) / 8;
		Param** validP = &mOrderedParams.front();

		switch(mNValidParams % 8) {
			case 0: (*validP++)->set(*vals++);
			case 7: (*validP++)->set(*vals++);
			case 6: (*validP++)->set(*vals++);
			case 5: (*validP++)->set(*vals++);
			case 4: (*validP++)->set(*vals++);
			case 3: (*validP++)->set(*vals++);
			case 2: (*validP++)->set(*vals++);
			case 1: (*validP++)->set(*vals++);
		}
		while(--n > 0) {
			(*validP++)->set(*vals++);
			(*validP++)->set(*vals++);
			(*validP++)->set(*vals++);
			(*validP++)->set(*vals++);
			(*validP++)->set(*vals++);
			(*validP++)->set(*vals++);
			(*validP++)->set(*vals++);
			(*validP++)->set(*vals++);
		}
	}
	
	// Set the substitutions of substituted parameters
	{
		int n = (mNSubstitutedParams+7) / 8;
		Param** substP = &mOrderedParams.back();

		switch(mNSubstitutedParams % 8) {
			case 0: (*substP--)->apply_substitution();
			case 7: (*substP--)->apply_substitution();
			case 6: (*substP--)->apply_substitution();
			case 5: (*substP--)->apply_substitution();
			case 4: (*substP--)->apply_substitution();
			case 3: (*substP--)->apply_substitution();
			case 2: (*substP--)->apply_substitution();
			case 1: (*substP--)->apply_substitution();
		}
		while(--n > 0) {
			(*substP--)->apply_substitution();
			(*substP--)->apply_substitution();
			(*substP--)->apply_substitution();
			(*substP--)->apply_substitution();
			(*substP--)->apply_substitution();
			(*substP--)->apply_substitution();
			(*substP--)->apply_substitution();
			(*substP--)->apply_substitution();
		}
	}
}