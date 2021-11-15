
#ifndef HORIZONTALITY_TOOL_HPP_
#define HORIZONTALITY_TOOL_HPP_

#include "Constraint_tool.hpp"

class Horizontality_tool : public Constraint_tool {
public:
	Horizontality_tool(Sketch_ws* env);

	std::string name() { return "horizontality"; }
// protected:
	int could_add_geom(SkGeometry* geom);

	void create_constraint(Constraint*& constr, SkGeometry*& priority_ent, Action_ptr& annotAct);
};

#endif