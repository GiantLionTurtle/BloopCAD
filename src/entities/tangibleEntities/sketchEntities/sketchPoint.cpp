
#include "sketchPoint.hpp"

#include <utils/errorLogger.hpp>

sketchPoint::sketchPoint(glm::vec2 pos_2d, plane_abstract_ptr basePlane_, bool immovable /*= false*/):
	sketchElement(basePlane_),
	point(point_abstract(glm::vec3(pos_2d, 0.0f), immovable))
{
	mRequire_VBUpdate = true;
	set_name("sketchPoint");
}

glm::vec3 sketchPoint::pos_vec() const
{
	return mBasePlane->point_2d_to_3d(glm::vec2(point_abstract::pos_vec()));
}

void sketchPoint::set_pos(glm::vec2 pos)
{
	point::set_pos(glm::vec3(pos, 0.0f));
}
void sketchPoint::set_pos(glm::vec3 pos)
{
	set_pos(glm::vec2(pos));
}
void sketchPoint::set_pos(point_abstract const& other)
{
	set_pos(other.pos_vec());
}