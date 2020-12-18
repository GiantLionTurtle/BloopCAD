
#ifndef POINT_ABSTRACT_HPP_
#define POINT_ABSTRACT_HPP_

#include <glm/glm.hpp>

class point_abstract {
protected:
	glm::vec3 mPos;
public:
	point_abstract(glm::vec3 pos): mPos(pos) {};
    virtual ~point_abstract() {};

	glm::vec3 pos() const { return mPos; }
	virtual void set_pos(glm::vec3 pos) { mPos = pos; }
	virtual void set_pos(point_abstract const& other) { mPos = other.mPos; }
};


#endif