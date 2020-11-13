
#include "startSketch_tool.hpp"

#include <workspaces/workspace.hpp>
#include <document.hpp>

#include <entities/part.hpp>

startSketch_tool::startSketch_tool(std::shared_ptr<std::shared_ptr<workspaceState>> workspaceState_):
    planeSelector_tool(workspaceState_)
{

}

bool startSketch_tool::manage_button_press(GdkEventButton* event)
{
    if((*mWorkspaceState)) {
        std::shared_ptr<entity> sketchPlane = entity_at_point(glm::vec2(event->x, (*mWorkspaceState)->doc->get_height() - event->y));

        if(sketchPlane) {
            std::shared_ptr<part> target = std::dynamic_pointer_cast<part>((*mWorkspaceState)->target);
            if(target) {
            }
            std::cout<<"Start sketch!\n";
        }
    }
    return true;
}