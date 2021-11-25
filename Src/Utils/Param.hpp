
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


#ifndef PARAM_HPP_
#define PARAM_HPP_

#include <Bloop/BaseObject.hpp>

class Constraint;

class Param : public DefaultBaseObject {
private:
	double mVal {0.0};
	int mFrozen { 0 };
	Param* mSubstitution { nullptr };
public:
	friend class Constraint;

	Param(double val)
		: mVal(val)
		, mSubstitution(nullptr)
	{}
	Param() = default;

	inline double val()
	{
		if(substituted()) 
			return mSubstitution->val();
		return mVal;
	}
	inline double& val_raw() { return mVal; }	
	inline void set(double val) { if(frozen() != 2) mVal = val; }

	inline void drag(double val) { set(val); set_frozen(1); }

	inline int frozen() { return mFrozen; }
	inline void set_frozen(int frozen) { if(mFrozen != 2) mFrozen = frozen; }

	inline bool substituted() { return mSubstitution != nullptr; }

	inline void set_substitution(Param* subst) 
	{
		if(substituted() && mSubstitution->weight() < subst->weight())
			mSubstitution->set_substitution(subst);
		
		mSubstitution = subst;
	}
	inline void apply_substitution() { if(substituted()) set(mSubstitution->val()); }
	inline void delete_substitution() { mSubstitution = nullptr; }

	inline int weight() { return substituted() ? mSubstitution->weight() : frozen(); }
};


class ParamIterator {
public:
	virtual int n_params() { return 0; }
	virtual Param* param(int ind) { return nullptr; }
};

/*
	@struct VarState describes a tuple between a variable and a value
*/
struct ParamState {
	/*
		@constructor VarState

		@param v The variable to set
		@param s The value tupled with the variable
	*/
	ParamState(Param* v, float s)
		: var(v)
		, st(s)
	{
		
	}
	ParamState()
		: var(nullptr)
		, st(0.0f)
	{

	}

	Param* var; // Variable
	float st;	// State
};

/*
	@struct VarDualState describes a tuple between a variable and two values
	(a before and an after state)
*/
struct ParamDualState {
	/*
		@constructor VarState

		@param v	The variable to set
		@param s1 	The "before" value
		@param s2 	The "after" value
	*/
	ParamDualState(Param* v, float s1, float s2)
		: var(v)
		, st1(s1)
		, st2(s2)
	{

	}
	ParamDualState()
		: var(nullptr)
		, st1(0.0f)
		, st2(0.0f)
	{

	}

	Param* var; // Variable
	float st1, st2; // Both states of the variable
};

#endif