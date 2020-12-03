
#include "line_tool.hpp"
#include <workspaces/workspace.hpp>
#include <entities/part.hpp>
#include <entities/sketch.hpp>

line_tool::line_tool(workspace* env):
	tool_abstract(env)
{
	
}

bool line_tool::manage_mouse_move(GdkEventMotion* event)
{
	// std::cout<<"updating line tool!\n";
	return true;
}
bool line_tool::manage_button_press(GdkEventButton* event)
{
	if(mEnv->state()) {
		std::shared_ptr<sketch> target = std::dynamic_pointer_cast<sketch>(mEnv->state()->target);

		if(target) {
			std::cout<<"vec: "<<glm::to_string(mEnv->state()->cam->front())<<"\n";
		}
	}
	return true;
}