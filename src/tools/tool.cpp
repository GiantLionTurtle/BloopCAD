
#include "tool.hpp"
#include <bloop.hpp>
#include <workspaces/workspace.hpp>

bool simpleSelector_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if((*mWorkspaceState)) {
		(*mWorkspaceState)->selectionBuffer->bind();
		unsigned char data[4] = {0, 0, 0, 0};
		GLCall(glReadPixels(event->x, (*mWorkspaceState)->height - event->y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data));
		std::shared_ptr<entity> ent = (*mWorkspaceState)->indexer->get(glm::ivec3(data[0], data[1], data[2]));
	
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

