
#ifndef STARTSKETCH_TOOL_HPP_
#define STARTSKETCH_TOOL_HPP_

#include <tools/common_tools/planeSelector_tool.hpp>

class startSketch_tool : public planeSelector_tool {
public:
	startSketch_tool(workspace* env);

	virtual void init();

   	virtual bool manage_button_press(GdkEventButton* event);
private:
	void start_sketch(std::shared_ptr<plane_abstract> sketchPlane, glm::vec3 camUp, glm::vec3 camRight);
};

#endif