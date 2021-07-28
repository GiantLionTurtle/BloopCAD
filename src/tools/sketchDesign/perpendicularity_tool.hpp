
#ifndef PERPENDICULARITY_TOOL_HPP_
#define PERPENDICULARITY_TOOL_HPP_

#include "constraint_tool.hpp"

class perpendicularity_tool : public constraint_tool {
public:
	perpendicularity_tool(sketchDesign* env);

	std::string name() { return "perpendicularity"; }
// protected:
	int could_add_entity(SkDrawable* ent);

	void create_constraint(SkConstraint*& constr, SkDrawable*& priority_ent);
};

#endif