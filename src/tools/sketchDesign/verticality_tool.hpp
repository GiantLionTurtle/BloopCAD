
#ifndef VERTICALITY_TOOL_HPP_
#define VERTICALITY_TOOL_HPP_

#include "constraint_tool.hpp"

class verticality_tool : public constraint_tool {
public:
	verticality_tool(sketchDesign* env);

	std::string name() { return "verticality_tool"; }
// protected:
	int could_add_entity(sketchEntity_ptr ent);

	void add_constraint_impl(std::shared_ptr<constraint_entity>& constr, sketchEntity_ptr& priority_ent);
};

#endif