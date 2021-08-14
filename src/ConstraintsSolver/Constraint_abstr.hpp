
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
	virtual double error(size_t ind)
	{
		return equ(ind)->eval();
	}
	virtual bool satisfied()
	{
		for(size_t i = 0; i < n_equs(); ++i) {
			if(std::abs(equ(i)->eval()) > 1e-12)
				return false;
		}
		return true;
	}
	virtual double derivative(var_ptr withRespectTo, size_t from_eq)
	{
		// TODO: would going through the constraint's variables to check if the target
		// is in the constraint save time? ; output zero if the variable is unknown to this constraint
		withRespectTo->set_as_var();
		double der = equ(from_eq)->d()->eval();
		withRespectTo->set_as_coef();
		return der;
	}
	double d(var_ptr withRespectTo, size_t from_eq = 0) { return derivative(withRespectTo, from_eq); }

	virtual equ_ptr equ(size_t ind) = 0;
	virtual size_t n_equs() = 0;

	virtual var_ptr var(size_t ind) = 0;
	virtual size_t n_vars() = 0;

	virtual std::string name() = 0;
};

#endif