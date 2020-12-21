
#include "sketch.hpp"
#include <utils/errorLogger.hpp>

sketch::sketch(std::shared_ptr<plane_abstract> base_plane):
    mBasePlane(base_plane)
{
    set_name("sketch");
}
sketch::sketch(std::shared_ptr<plane_abstract> base_plane, entity* parent):
    mBasePlane(base_plane),
    entity(parent) // Follow that entity
{
    set_name("sketch");
}

void sketch::draw_impl(std::shared_ptr<camera> cam, int frame)
{

}
void sketch::draw_selection_impl(std::shared_ptr<camera> cam, int frame)
{

}