
#ifndef HORIZONTALITY_TOOL_HPP_
#define HORIZONTALITY_TOOL_HPP_

#include "constraint_tool.hpp"

class horizontality_tool : public constraint_tool {
public:
	horizontality_tool(sketchDesign* env);

	void init();

	std::string name() { return "horizontality"; }
protected:
	int could_add_entity(sketchEntity_ptr ent);

	void add_constraint();
};

#endif