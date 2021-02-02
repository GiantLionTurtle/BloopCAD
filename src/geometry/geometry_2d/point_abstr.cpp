
#include "point_abstr.hpp"

#include <glm/gtx/string_cast.hpp>

namespace geom_2d {

point_abstr::point_abstr(glm::vec2 pos, bool immovable /*= false*/): 
	geometry_2d_abstr(2),
	mPos(new variableVector2(pos)) 
{
	if(immovable) {
		set_constant();
	}
}
point_abstr::point_abstr(variableVector2_ptr pos, bool immovable /*= false*/): 
	geometry_2d_abstr(2),
	mPos(pos) 
{
	if(immovable) {
		set_constant();
	}
}
point_abstr::~point_abstr()
{

}

void point_abstr::set(glm::vec2 const& pos) 
{
	mPos->set(pos);
	post_set_behavior();
}
void point_abstr::set(variableVector2_ptr pos)
{
	mPos = pos;
	post_set_behavior();
}
void point_abstr::set(point_abstr_ptr other)
{
	mPos->set(other->vec());
	post_set_behavior();
}
void point_abstr::set(point_abstr const& other)
{
	mPos->set(other.vec());
	post_set_behavior();
}

void point_abstr::set_constant()
{
	mPos->set_constant();
}
void point_abstr::set_tmpConstant(bool const_)
{
	mPos->set_tmpConstant(const_);
}

float point_abstr::dist_to_point(glm::vec2 const& pt)
{
	return glm::length(vec() - pt);
}
float point_abstr::dist_to_point_2(glm::vec2 const& pt)
{
	glm::vec2 d = vec() - pt;
	return d.x * d.x + d.y * d.y;
}

std::ostream& operator<<(std::ostream& os, point_abstr_ptr p)
{
	os<<*p.get();
	return os;
}
std::ostream& operator<<(std::ostream& os, point_abstr const& p)
{
	os<<glm::to_string(p.vec());
	return os;
}

point_abstr_ptr operator+(point_abstr_ptr p, glm::vec2 v)
{
	return std::make_shared<point_abstr>(p->vec() + v);
}
point_abstr_ptr operator-(point_abstr_ptr p, glm::vec2 v)
{
	return std::make_shared<point_abstr>(p->vec() - v);
}

} // !geom_2d