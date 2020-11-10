
#ifndef STARTSKETCH_TOOL_HPP_
#define STARTSKETCH_TOOL_HPP_

#include <tools/utils/planeSelector_tool.hpp>

class startSketch_tool : public planeSelector_tool {
public:
	startSketch_tool(std::shared_ptr<std::shared_ptr<workspaceState>> workspaceState_);

   	virtual bool manage_button_press(GdkEventButton* event);
};

#endif