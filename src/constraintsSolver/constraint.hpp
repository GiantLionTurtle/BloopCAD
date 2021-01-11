
#ifndef CONSTRAINT_HPP_
#define CONSTRAINT_HPP_

#include "expression.hpp"
#include "entities/geometry/point_abstract.hpp"

#include <vector>

class constraint {
protected:
    std::vector<expression_ptr> mEquations;
    std::vector<variable_ptr> mVariables;
public:
    constraint();
    constraint(std::vector<expression_ptr> equs, std::vector<variable_ptr> vars);

    std::vector<expression_ptr> equations() const { return mEquations; }
    std::vector<variable_ptr> variables() const { return mVariables; }
};

class coincidence_constraint : public constraint {
public:
    coincidence_constraint(point_abstract_ptr p1, point_abstract_ptr p2);
};

#endif