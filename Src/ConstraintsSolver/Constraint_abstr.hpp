
#ifndef CONSTRAINT_ABSTR_HPP_
#define CONSTRAINT_ABSTR_HPP_

#include "Expression_forward.hpp"
#include <Utils/Debug_util.hpp>
#include <BaseObject.hpp>

#include <array>

/*
	@class Constraint_abstr represents a constraint that is a collection of at least one
	equation

	@parent BaseObject
*/
class Constraint_abstr : virtual public BaseObject {
public:
	Constraint_abstr();
	~Constraint_abstr();

	/*
		@function error gives an indexed error

		@oaram ind The index of the equation from which to pull the error
		@return The error at this index
	*/
	virtual double error(size_t ind);
	/*
		@function satisfied

		@return If all the equations in the constraint have an error below a (currently arbitrary) treshold
	*/
	virtual bool satisfied();
	/*
		@function derivative gives a partial derivative for a variable for an equation

		@param withRespectTo 				The variable that the equation must be partially derived for
		@param from_eq  [defaults to 0]		The index of the equation that must be derived
		@note @param from_eq defaults to 0 because most constraint have 1 equation
	*/
	virtual double derivative(var_ptr withRespectTo, size_t from_eq = 0);
	/*
		@function d Is a wrapper for @function derivative
	*/
	double d(var_ptr withRespectTo, size_t from_eq = 0) { return derivative(withRespectTo, from_eq); }

	/*
		@function equ is a getter to access equations of the constraint

		@param ind [defaults to 0] Is the index of the wanted equation
		@note @param ind defaults to 0 because most constraint have 1 equation
	*/
	virtual equ_ptr equ(size_t ind) = 0;
	/*
		@function n_equs

		@return The number of equations contained (most times it will be 0)
	*/
	virtual size_t n_equs() = 0;

	/*
		@function var is a getter to access variables of the constraint

		@param ind Is the index of the wanted variable
	*/
	virtual var_ptr var(size_t ind) = 0;
	/*
		@function n_equs

		@return The number of equations contained (most times it will be 0)		
	*/
	virtual size_t n_vars() = 0;

	/*
		@function name

		@return The name of the constraint
		@note This is mostly for debug
	*/
	virtual std::string name() = 0;
};

#endif