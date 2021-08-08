
#include "SkLine.hpp"

SkLine::SkLine(glm::vec2 ptA, glm::vec2 ptB, Geom3d::plane_abstr* pl, bool fixed_):
	SkHandleCurve({ ptA, ptB }, pl, fixed_)
{
	set_name("SkLine");
}
