
#ifndef COINCIDENCE_TOOL_HPP_
#define COINCIDENCE_TOOL_HPP_

#include "constraint_tool.hpp"

class coincidence_tool : public constraint_tool {
public:
	coincidence_tool(sketchDesign* env);

	virtual void init();

	std::string name() { return "coincidence"; }
protected:
	int could_add_entity(sketchEntity_ptr ent);

	void add_constraint();
};

#endif