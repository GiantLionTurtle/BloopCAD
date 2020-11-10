
#include "planeSelector_tool.hpp"
#include <entities/plane_abstract.hpp>

planeSelector_tool::planeSelector_tool(std::shared_ptr<std::shared_ptr<workspaceState>> workspaceState_):
    simpleSelector_tool(workspaceState_)
{

}

std::shared_ptr<entity> planeSelector_tool::entity_at_point(glm::vec2 point)
{
    std::shared_ptr<entity> hovered_ent = primitive_entity_at_point(point);

    if(hovered_ent && std::dynamic_pointer_cast<plane_abstract>(hovered_ent)) {
        return hovered_ent;
    }
    return nullptr;
}
