
#ifndef PARAM_HPP_
#define PARAM_HPP_

#include <BaseObject.hpp>

class Constraint_abstr;

class Param : public DefaultBaseObject {
private:
	double mVal {0.0};
	int mFrozen { 0 };
	Param* mSubstitution { nullptr };
public:
	friend class Constraint_abstr;

	Param(double val)
		: mVal(val)
		, mSubstitution(nullptr)
	{}
	Param() = default;

	inline double val() { return mVal; }
	inline double& val_raw() { return mVal; }	
	inline void set(double val) { mVal = val; }

	inline void drag(double val) { set_frozen(1); set(val); }

	inline int frozen() { return mFrozen; }
	inline void set_frozen(int frozen) { if(mFrozen != 2) mFrozen = frozen; }

	inline bool substituted() { return mSubstitution != nullptr; }

	inline void set_substitution(Param* subst) { mSubstitution = subst; }
	inline void apply_substitution() { if(substituted()) set(mSubstitution->val()); }
	inline void delete_substitution() { mSubstitution = nullptr; }

	inline int weight() { return substituted() + 2 * frozen(); }
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