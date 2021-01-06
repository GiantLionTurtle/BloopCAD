
#ifndef POINT_ABSTRACT_HPP_
#define POINT_ABSTRACT_HPP_

#include <constraintsSolver/expressionVector.hpp>

#include <glm/glm.hpp>

class point_abstract {
protected:
	variableVector3_ptr mPos;
public:
	point_abstract(glm::vec3 pos): mPos(new variableVector3(pos)) {};
    virtual ~point_abstract() {};

	glm::vec3 pos() const { return mPos->get(); }
	virtual void set_pos(glm::vec3 const& pos) { mPos->set(pos); }
	virtual void set_pos(point_abstract const& other) { mPos = other.mPos; }
};


#endif