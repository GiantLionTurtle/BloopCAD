
#ifndef POINT_ABSTRACT_HPP_
#define POINT_ABSTRACT_HPP_

#include <constraintsSolver/expressionVector.hpp>

#include <glm/glm.hpp>

class point_abstract {
protected:
	variableVector3_ptr mPos;
public:
	point_abstract(glm::vec3 pos): mPos(new variableVector3(pos)) {}
	point_abstract(variableVector3_ptr pos): mPos(pos) {}
    virtual ~point_abstract() {}

	glm::vec3 pos_vec() const { return mPos->get(); }
	variableVector3_ptr pos_var() const { return mPos; }
	virtual void set_pos(glm::vec3 const& pos) { mPos->set(pos); }
	virtual void set_pos(variableVector3_ptr pos) { mPos = pos; }
	virtual void set_pos(std::shared_ptr<point_abstract> other) { mPos->set(other->pos_vec()); }
};


#endif