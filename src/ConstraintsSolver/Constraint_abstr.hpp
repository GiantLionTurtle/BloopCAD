
#ifndef CONSTRAINT_ABSTR_HPP_
#define CONSTRAINT_ABSTR_HPP_

#include "Expression.hpp"
#include <utils/DebugUtils.hpp>
#include <baseObject.hpp>

#include <array>

class Constraint_abstr : virtual public baseObject {
public:
	Constraint_abstr() {}
	~Constraint_abstr() {}
	virtual double error(size_t eq) = 0;
	virtual bool satisfied() = 0;
	virtual double derivative(var_ptr withRespectTo, size_t from_eq) = 0;
	double d(var_ptr withRespectTo, size_t from_eq = 0) { return derivative(withRespectTo, from_eq); }

	virtual equ_ptr equ(size_t ind) = 0;
	virtual size_t n_equs() = 0;

	virtual var_ptr var(size_t ind) = 0;
	virtual size_t n_vars() = 0;

	virtual std::string name() = 0;
};

#endif