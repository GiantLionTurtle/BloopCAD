
#include "point_abstract.hpp"

#include <glm/gtx/string_cast.hpp>

point_abstract::point_abstract(glm::vec3 pos, bool immovable /*= false*/): 
	mPos(new variableVector3(pos)) 
{
	if(immovable) {
		set_constant();
	}
}
point_abstract::point_abstract(variableVector3_ptr pos, bool immovable /*= false*/): 
	mPos(pos) 
{
	if(immovable) {
		set_constant();
	}
}

void point_abstract::set_pos(glm::vec3 const& pos) 
{
	mPos->set(pos);
	post_set_update();
}
void point_abstract::set_pos(variableVector3_ptr pos)
{
	mPos = pos;
	post_set_update();
}
void point_abstract::set_pos(point_abstract_ptr other)
{
	mPos->set(other->pos_vec());
	post_set_update();
}
void point_abstract::set_constant()
{
	mPos->set_constant();
}
void point_abstract::set_tmpConstant(bool immovable)
{
	mPos->set_tmpConstant(immovable);
}

float point_abstract::dist(point_abstract const& pt)
{
	return glm::length(pos_vec() - pt.pos_vec());
}
float point_abstract::dist2(point_abstract const& pt)
{
	glm::vec3 d = pos_vec() - pt.pos_vec();
	return d.x * d.x + d.y * d.y + d.z * d.z;
}

std::ostream& operator<<(std::ostream& os, point_abstract_ptr p)
{
	os<<*p.get();
	return os;
}
std::ostream& operator<<(std::ostream& os, point_abstract const& p)
{
	os<<glm::to_string(p.pos_vec());
	return os;
}

point_abstract_ptr operator+(point_abstract_ptr p, glm::vec3 v)
{
	return std::make_shared<point_abstract>(p->pos_vec() + v);
}
point_abstract_ptr operator-(point_abstract_ptr p, glm::vec3 v)
{
	return std::make_shared<point_abstract>(p->pos_vec() - v);
}