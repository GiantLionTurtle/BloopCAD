
#ifndef EQUATIONSSYSTEM_HPP_
#define EQUATIONSSYSTEM_HPP_

#include "expression.hpp"

#include <vector>

class equationsSystem {
public:
	static constexpr double kEpsilon = 1e-10;
private:
	std::vector<expression_ptr> mEquations;
public:
	equationsSystem();
	equationsSystem(std::vector<expression_ptr> eqs);

	void add_equation(expression_ptr eq);
	void add_equations(std::vector<expression_ptr> eqs);

	size_t size() const { return mEquations.size(); };

	bool satisfied();
};

#endif