
#include "simpleSelector_tool.hpp"

#include <graphics_utils/GLCall.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>

simpleSelector_tool::simpleSelector_tool(std::shared_ptr<std::shared_ptr<workspaceState>> workspaceState_): 
    tool_abstract(workspaceState_),
    mCurrentHover(nullptr) 
{
    
}

bool simpleSelector_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if((*mWorkspaceState)) {

		std::shared_ptr<entity> ent = entity_at_point(glm::vec2(event->x, (*mWorkspaceState)->doc->get_height() - event->y));
	
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

std::shared_ptr<entity> simpleSelector_tool::primitive_entity_at_point(glm::vec2 point)
{
    if((*mWorkspaceState)) {
		(*mWorkspaceState)->doc->selectionBuffer()->bind();
		unsigned char data[4] = {0, 0, 0, 0};
		GLCall(glReadPixels(point.x, point.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data));
		std::shared_ptr<entity> ent = (*mWorkspaceState)->doc->indexer()->get(glm::ivec3(data[0], data[1], data[2]));
        return ent;
	}
    return nullptr;
}