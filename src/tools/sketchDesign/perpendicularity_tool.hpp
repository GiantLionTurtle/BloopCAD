
#ifndef PERPENDICULARITY_TOOL_HPP_
#define PERPENDICULARITY_TOOL_HPP_

#include "constraint_tool.hpp"

class perpendicularity_tool : public constraint_tool {
public:
	perpendicularity_tool(sketchDesign* env);

	void init();

	std::string name() { return "perpendicularity"; }
protected:
	int could_add_entity(sketchEntity_ptr ent);

	void add_constraint();
};

#endif