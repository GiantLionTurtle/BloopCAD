
#ifndef COINCIDENCE_TOOL_HPP_
#define COINCIDENCE_TOOL_HPP_

#include "Constraint_tool.hpp"

class Coincidence_tool : public Constraint_tool {
public:
	Coincidence_tool(Sketch_ws* env);

	std::string name() { return "coincidence"; }
// protected:
	int could_add_geom(SkGeometry* geom);

	void create_constraint(Constraint_abstr*& constr, SkGeometry*& priority_ent, Action_ptr& annotAct);
};

#endif