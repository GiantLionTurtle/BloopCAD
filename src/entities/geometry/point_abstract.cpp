
#include "point_abstract.hpp"

point_abstract::point_abstract(glm::vec3 pos, bool immovable /*= false*/): 
	mPos(new variableVector3(pos)) 
{
	if(immovable) {
		mPos->set_constant(true);
	}
}
point_abstract::point_abstract(variableVector3_ptr pos, bool immovable /*= false*/): 
	mPos(pos) 
{
	if(immovable) {
		mPos->set_constant(true);
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