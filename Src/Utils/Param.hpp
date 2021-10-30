
#ifndef PARAM_HPP_
#define PARAM_HPP_

class Constraint_abstr;

class Param {
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
	inline void set(double val) { mVal = val; }

	inline int frozen() { return mFrozen; }
	inline void set_frozen(int frozen) { mFrozen = frozen; }

	inline bool substituted() { return mSubstitution != nullptr; }

	inline void set_substitution(Param* subst) { mSubstitution = subst; }
	inline void apply_substitution() { set(mSubstitution->val()); }
	inline void delete_substitution() { mSubstitution = nullptr; }
};

#endif