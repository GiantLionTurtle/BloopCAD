
#include "sketchPoint.hpp"

#include <utils/errorLogger.hpp>

sketchPoint::sketchPoint(std::shared_ptr<plane_abstract> basePlane_, glm::vec2 pos_2d):
	sketchEntity(basePlane_),
	point(basePlane_->point_2d_to_3d(pos_2d))
{
	mRequire_VBUpdate = true;
}

void sketchPoint::set_pos(glm::vec2 pos)
{
	point::set_pos(mBasePlane->point_2d_to_3d(pos));
}
void sketchPoint::set_pos(glm::vec3 pos)
{
	set_pos(glm::vec2(pos));
}
void sketchPoint::set_pos(point_abstract const& other)
{
	set_pos(other.pos_vec());
}