
#include "sketch.hpp"
#include <utils/errorLogger.hpp>

sketch::sketch(plane_abstract_ptr base_plane):
    mBasePlane(base_plane)
{
    set_name("sketch");
}
sketch::sketch(plane_abstract_ptr base_plane, entity* parent):
    mBasePlane(base_plane),
    entity(parent) // Follow that entity
{
    set_name("sketch");
}

void sketch::draw_impl(camera_ptr cam, int frame)
{

}
void sketch::draw_selection_impl(camera_ptr cam, int frame)
{

}