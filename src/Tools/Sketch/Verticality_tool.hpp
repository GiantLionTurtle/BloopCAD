
#ifndef VERTICALITY_TOOL_HPP_
#define VERTICALITY_TOOL_HPP_

#include "Constraint_tool.hpp"

class Verticality_tool : public Constraint_tool {
public:
	Verticality_tool(sketchDesign* env);

	std::string name() { return "Verticality_tool"; }
// protected:
	int could_add_geom(SkDrawable* geom);

	void create_constraint(SkConstraint*& constr, SkDrawable*& priority_ent);
};

#endif