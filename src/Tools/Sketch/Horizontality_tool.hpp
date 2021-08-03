
#ifndef HORIZONTALITY_TOOL_HPP_
#define HORIZONTALITY_TOOL_HPP_

#include "Constraint_tool.hpp"

class Horizontality_tool : public Constraint_tool {
public:
	Horizontality_tool(sketchDesign* env);

	std::string name() { return "horizontality"; }
// protected:
	int could_add_entity(SkDrawable* ent);

	void create_constraint(SkConstraint*& constr, SkDrawable*& priority_ent);
};

#endif