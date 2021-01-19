
#include "sketchPoint.hpp"

#include <utils/errorLogger.hpp>

sketchPoint::sketchPoint(glm::vec2 pos_2d, plane_abstract_ptr basePlane_, bool immovable /*= false*/):
	sketchElement(basePlane_),
	point(point_abstract(glm::vec3(pos_2d, 0.0f), immovable))
{
	mRequire_VBUpdate = true;
	set_name("sketchPoint");
}

glm::vec3 sketchPoint::pos_val() const
{
	return mBasePlane->point_2d_to_3d(glm::vec2(point_abstract::pos_val()));
}

void sketchPoint::set(glm::vec2 pos)
{
	point::set(glm::vec3(pos, 0.0f));
}
void sketchPoint::set(glm::vec3 pos)
{
	set(glm::vec2(pos));
}
void sketchPoint::set(point_abstract const& other)
{
	set(other.pos_val());
}

std::vector<variable_ptr> sketchPoint::variables()
{
	return { mPos->x, mPos->y };
}

subEquationsSystem sketchPoint::coincidence()
{
	return { {}, { mPos->x->expr(), mPos->y->expr() } };
}
std::vector<subEquationsSystem> sketchPoint::verticality()
{
	return { { {}, { mPos->x->expr() } } };
}
std::vector<subEquationsSystem> sketchPoint::horizontality()
{
	return { { {}, { mPos->y->expr() } } };
}