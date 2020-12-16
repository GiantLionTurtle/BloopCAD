
#include "sketch.hpp"
#include <utils/errorLogger.hpp>

sketch::sketch(std::shared_ptr<plane_abstract> base_plane):
    mBasePlane(base_plane)
{

}
sketch::sketch(std::shared_ptr<plane_abstract> base_plane, entity* parent):
    mBasePlane(base_plane),
    entity(parent) // Follow that entity
{

}

void sketch::draw_impl(std::shared_ptr<camera> cam, int frame)
{
    for_each([cam, frame](std::shared_ptr<entity> ent) { ent->draw(cam, frame); }); // Call the draw function for each sub-entity
}
void sketch::draw_selection_impl(std::shared_ptr<camera> cam, int frame)
{
    for_each([cam, frame](std::shared_ptr<entity> ent) { ent->draw_selection(cam, frame); }); // Call the draw_selection function for each sub-entity
}