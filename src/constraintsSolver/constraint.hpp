
#ifndef CONSTRAINT_HPP_
#define CONSTRAINT_HPP_

#include "expression.hpp"
#include <utils/DebugUtils.hpp>
#include <baseObject.hpp>

#include <array>

class constraint_abstr : virtual public baseObject {
public:
	constraint_abstr() {}
	~constraint_abstr() {}
	virtual double error(size_t eq) = 0;
	virtual bool satisfied() = 0;
	virtual double derivative(var_ptr withRespectTo_var, size_t from_eq) = 0;
	double d(var_ptr withRespectTo, size_t from_eq = 0) { return derivative(withRespectTo, from_eq); }

	virtual equ_ptr equ(size_t ind) = 0;
	virtual size_t n_equs() = 0;

	virtual var_ptr var(size_t ind) = 0;
	virtual size_t n_vars() = 0;

	virtual std::string name() = 0;
};

class constraint_abstract : public defaultBaseObject {
public:
	enum constraint_types { NONE, COINCIDENCE, PERPENDICULARITY, PARALLELISM, HORIZONTALITY, VERTICALITY }; // high level representation (for little annotations and such)
protected:
	int mType;//, mTag;
public:
	constraint_abstract(int type):
		// mTag(0),
		mType(type)
	{}
	virtual double error(size_t eq) = 0;
	virtual bool satisfied() = 0;
	virtual double derivative(var_ptr withRespectTo_var, size_t from_eq) = 0;
	double d(var_ptr withRespectTo, size_t from_eq = 0) { return derivative(withRespectTo, from_eq); }

	virtual equ_ptr equ(size_t ind) = 0;
	virtual size_t n_equs() = 0;

	virtual var_ptr var(size_t ind) = 0;
	virtual size_t n_vars() = 0;

	virtual std::string name() = 0;

	void set_type(int tp) { mType = tp; }
	int type() { return mType; }
};

#endif