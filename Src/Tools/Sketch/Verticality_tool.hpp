
#ifndef VERTICALITY_TOOL_HPP_
#define VERTICALITY_TOOL_HPP_

#include "Constraint_tool.hpp"

class Verticality_tool : public Constraint_tool {
public:
	Verticality_tool(Sketch_ws* env);

	std::string name() { return "Verticality_tool"; }
// protected:
	int could_add_geom(SkDrawable* geom);

	void create_constraint(Constraint_abstr*& constr, SkDrawable*& priority_ent);
};

#endif