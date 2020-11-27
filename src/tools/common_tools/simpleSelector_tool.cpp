
#include "simpleSelector_tool.hpp"

#include <graphics_utils/GLCall.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

simpleSelector_tool::simpleSelector_tool(workspace* env): 
	tool_abstract(env),
	mCurrentHover(nullptr) 
{
	
}

bool simpleSelector_tool::manage_button_press(GdkEventButton* event)
{
	if(mEnv->state()) {
		mEnv->state()->doc->toggle_select(
			id_at_point(glm::vec2(event->x, mEnv->state()->doc->get_height() - event->y)), 
			{mEnv->state()->cam->up(), mEnv->state()->cam->right()},
			event->state & GDK_CONTROL_MASK || event->state & GDK_SHIFT_MASK);		
	}
	return true;
}
bool simpleSelector_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(mEnv->state()) {
		std::shared_ptr<entity> ent = entity_at_point(glm::vec2(event->x, mEnv->state()->doc->get_height() - event->y));
	
		if(ent != mCurrentHover) {
			if(mCurrentHover) {
				mCurrentHover->set_hover(false);
			}
			if(ent) {
				ent->set_hover(true);
			}
			mCurrentHover = ent;
		}
	}
	return true;
}

std::shared_ptr<entity> simpleSelector_tool::entity_at_point(glm::vec2 point)
{
	return primitive_entity_at_point(point);
}

glm::ivec3 simpleSelector_tool::id_at_point(glm::vec2 point)
{
	if(mEnv->state()) {
		mEnv->state()->doc->selectionBuffer()->bind();
		unsigned char data[4] = {0, 0, 0, 0};
		GLCall(glReadPixels(point.x, point.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data));
		return glm::ivec3(data[0], data[1], data[2]);
	}
	return glm::vec3(0, 0, 0);
}
std::shared_ptr<entity> simpleSelector_tool::primitive_entity_at_point(glm::vec2 point)
{
	if(mEnv->state()) {
		return mEnv->state()->doc->indexer()->get(id_at_point(point));
	}
	return nullptr;
}
