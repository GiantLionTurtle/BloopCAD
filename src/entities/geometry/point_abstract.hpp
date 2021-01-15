
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
	point_abstract(glm::vec3 pos, bool immovable = false);
	point_abstract(variableVector3_ptr pos, bool immovable = false);
    virtual ~point_abstract() {}

	virtual glm::vec3 pos_vec() const { return mPos->get(); }
	virtual variableVector3_ptr pos_var() const { return mPos; }
	virtual void set_pos(glm::vec3 const& pos);
	virtual void set_pos(variableVector3_ptr pos);
	virtual void set_pos(point_abstract_ptr other);

	void set_constant();
	void set_tmpConstant(bool const_);

	float dist(point_abstract const& pt);
	float dist(point_abstract_ptr pt) { return dist(*pt.get()); }
	float dist2(point_abstract const& pt);
	float dist2(point_abstract_ptr pt) { return dist2(*pt.get()); }
protected:
	virtual void post_set_update() {}
};

std::ostream& operator<<(std::ostream& os, point_abstract_ptr p);
std::ostream& operator<<(std::ostream& os, point_abstract const& p);

point_abstract_ptr operator+(point_abstract_ptr p, glm::vec3 v);
point_abstract_ptr operator-(point_abstract_ptr p, glm::vec3 v);

#endif