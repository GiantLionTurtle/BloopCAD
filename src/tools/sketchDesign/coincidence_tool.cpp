
#include "coincidence_tool.hpp"

#include <constraintsSolver/constraint.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

coincidence_tool::coincidence_tool(workspace* env):
	simpleSelector_tool(env)
{

}

void coincidence_tool::init()
{
	mStarted = false;
}

bool coincidence_tool::manage_button_press(GdkEventButton* event)
{
	if(!mEnv) {
		LOG_WARNING("No valid workspace.");
		return true;
	}
	
	point_abstract_ptr pt = std::dynamic_pointer_cast<point_abstract>
	(primitive_entity_at_point(glm::vec2(event->x, mEnv->state()->doc->get_height() - event->y)));

	if(pt) {
		if(!mStarted) {
			mPointA = pt;
			mStarted = true;
		} else {
			mPointB = pt;
			mStarted = false;
			sketch_ptr sk = std::dynamic_pointer_cast<sketch>(mEnv->state()->target);
			if(!sk) {
				LOG_WARNING("No valid sketch.");
				return true;
			}
			if(sk->add_constraint(std::make_shared<coincidence_constraint>(mPointA, mPointB))) {
				std::cout<<"Successfully added constraint!\n";
			}
		}
	}
	return true;
}