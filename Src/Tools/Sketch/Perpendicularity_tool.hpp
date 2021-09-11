
#ifndef PERPENDICULARITY_TOOL_HPP_
#define PERPENDICULARITY_TOOL_HPP_

#include "Constraint_tool.hpp"

class Perpendicularity_tool : public Constraint_tool {
public:
	Perpendicularity_tool(Sketch_ws* env);

	std::string name() { return "perpendicularity"; }
// protected:
	int could_add_geom(SkDrawable* geom);

	void create_constraint(SkConstraint*& constr, SkDrawable*& priority_ent);
};

#endif