
#ifndef CONSTRAINT_ABSTR_HPP_
#define CONSTRAINT_ABSTR_HPP_

#include "Expression_forward.hpp"
#include <Utils/Debug_util.hpp>
#include <BaseObject.hpp>

#include <array>

class Constraint_abstr : virtual public BaseObject {
public:
	Constraint_abstr();
	~Constraint_abstr();
	virtual double error(size_t ind);
	virtual bool satisfied();
	virtual double derivative(var_ptr withRespectTo, size_t from_eq);
	double d(var_ptr withRespectTo, size_t from_eq = 0);

	virtual equ_ptr equ(size_t ind) = 0;
	virtual size_t n_equs() = 0;

	virtual var_ptr var(size_t ind) = 0;
	virtual size_t n_vars() = 0;

	virtual std::string name() = 0;
};

#endif