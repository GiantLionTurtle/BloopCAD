
#include "planeSelector_tool.hpp"
#include <entities/geometry/plane_abstract.hpp>
#include <entities/entity.hpp>

planeSelector_tool::planeSelector_tool(workspace* env):
    simpleSelector_tool(env)
{

}

bool planeSelector_tool::should_hover(entity_ptr ent)
{
    return entity_valid(ent);
}

entity_ptr planeSelector_tool::entity_at_point(glm::vec2 point)
{
    entity_ptr hovered_ent = primitive_entity_at_point(point);
    if(hovered_ent && entity_valid(hovered_ent)) { // Check if a dynamic cast is valid
        return hovered_ent;
    }
    return nullptr;
}

bool planeSelector_tool::entity_valid(entity_ptr ent)
{
    if(std::dynamic_pointer_cast<plane_abstract>(ent))
        return true;
    return false;
}