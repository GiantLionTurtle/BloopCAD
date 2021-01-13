
#ifndef POINT_ABSTRACT_HPP_
#define POINT_ABSTRACT_HPP_

#include <constraintsSolver/expressionVector.hpp>

#include <glm/glm.hpp>

class point_abstract;
using point_abstract_ptr = std::shared_ptr<point_abstract>;

class point_abstract {
protected:
	variableVector3_ptr mPos;
public:
	point_abstract(glm::vec3 pos): mPos(new variableVector3(pos)) {}
	point_abstract(variableVector3_ptr pos): mPos(pos) {}
    virtual ~point_abstract() {}

	virtual glm::vec3 pos_vec() const { return mPos->get(); }
	virtual variableVector3_ptr pos_var() const { return mPos; }
	virtual void set_pos(glm::vec3 const& pos) { mPos->set(pos); post_set_update(); }
	virtual void set_pos(variableVector3_ptr pos) { mPos = pos; post_set_update(); }
	virtual void set_pos(point_abstract_ptr other) { mPos->set(other->pos_vec()); post_set_update(); }
protected:
	virtual void post_set_update() {}
};


#endif