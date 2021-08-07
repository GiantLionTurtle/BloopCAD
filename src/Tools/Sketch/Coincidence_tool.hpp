
#ifndef COINCIDENCE_TOOL_HPP_
#define COINCIDENCE_TOOL_HPP_

#include "Constraint_tool.hpp"

class Coincidence_tool : public Constraint_tool {
public:
	Coincidence_tool(sketchDesign* env);

	std::string name() { return "coincidence"; }
// protected:
	int could_add_geom(SkDrawable* geom);

	void create_constraint(SkConstraint*& constr, SkDrawable*& priority_ent);
};

#endif