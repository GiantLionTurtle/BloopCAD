
#ifndef VERTICALITY_TOOL_HPP_
#define VERTICALITY_TOOL_HPP_

#include "constraint_tool.hpp"

class verticality_tool : public constraint_tool {
public:
	verticality_tool(sketchDesign* env);

	std::string name() { return "verticality_tool"; }
// protected:
	int could_add_entity(SkDrawable* ent);

	void create_constraint(SkConstraint*& constr, SkDrawable*& priority_ent);
};

#endif