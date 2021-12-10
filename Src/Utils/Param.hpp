
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

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
#include <Utils/Debug_util.hpp>

class Constraint;
class SubstBlob;

class Param : public DefaultBaseObject {
public:
	// PRETTYFIXED, not a great name, means it is fixed for it to be a prettier solve, but is not
	// as enforced as DRAGGED (for instance the opposing point of a DRAGGED handle on a line would
	// have this level)
	enum Frozen_levels { DEAD = 0, UNFROZEN = 1, PRETTYFIXED = 2, DRAGGED = 3, GEOMETRICALLYFROZEN = 4, FIXED = 5 };
protected:
	double mVal { 0.0 };
	double* mValOrigin;
	int mFrozen { 0 };
	SubstBlob* mBlob { nullptr };
public:
	friend class Constraint;

	Param(double val)
		: mVal(val)
		, mValOrigin(&mVal)
	{

	}
	Param()
		: mVal(0.0)
		, mValOrigin(&mVal)
	{

	}
	Param(Param const& p)
	{
		mVal = p.mVal;
		mValOrigin = p.mValOrigin;
		mFrozen = p.mFrozen;
		mBlob = p.mBlob;
	}
	Param& operator=(Param const& p)
	{
		mVal = p.mVal;
		mValOrigin = p.mValOrigin;
		mFrozen = p.mFrozen;
		mBlob = p.mBlob;
		return *this;
	}

	inline double* origin() { return mValOrigin; }
	inline double val() { return *mValOrigin; }
	inline double val_internal() { return mVal; }
	inline void set(double val) { if(frozen() < Frozen_levels::GEOMETRICALLYFROZEN) mVal = val; }
	void set_origin(double* orig) { mValOrigin = orig; }
	void reset_origin(bool keepValue)
	{
		if(keepValue)
			set(val());
		mValOrigin = &mVal;
	}

	SubstBlob* blob() { return mBlob; }
	void set_blob(SubstBlob* b) { mBlob = b; }

	inline void drag(double val) { set(val); set_frozen(Param::Frozen_levels::DRAGGED); }

	inline int frozen() { return mFrozen; }
	inline void set_frozen(int frozen) { if(mFrozen < Frozen_levels::GEOMETRICALLYFROZEN) mFrozen = frozen; }

	inline int weight() { return frozen() * mExists; }
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