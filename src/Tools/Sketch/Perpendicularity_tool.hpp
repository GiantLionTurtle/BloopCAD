
#ifndef PERPENDICULARITY_TOOL_HPP_
#define PERPENDICULARITY_TOOL_HPP_

#include "Constraint_tool.hpp"

class Perpendicularity_tool : public Constraint_tool {
public:
	Perpendicularity_tool(sketchDesign* env);

	std::string name() { return "perpendicularity"; }
// protected:
	int could_add_entity(SkDrawable* ent);

	void create_constraint(SkConstraint*& constr, SkDrawable*& priority_ent);
};

#endif