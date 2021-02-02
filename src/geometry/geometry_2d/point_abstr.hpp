
#ifndef POINT_ABSTR2_HPP_
#define POINT_ABSTR2_HPP_

#include <constraintsSolver/expressionVector2.hpp>
#include "geometry_2d_abstr.hpp"

#include <glm/glm.hpp>

namespace geom_2d {

class point_abstr : public geometry_2d_abstr {
protected:
	variableVector2_ptr mPos;
public:
	point_abstr(glm::vec2 pos, bool immovable = false);
	point_abstr(variableVector2_ptr pos, bool immovable = false);
    virtual ~point_abstr();

	virtual glm::vec2 vec() const { return mPos->get(); }
	// virtual glm::vec3 vec_world() const { return mBasePlane->point_2d_to_3d(mPos->get()); }
	virtual variableVector2_ptr var() const { return mPos; }

	virtual void set(glm::vec2 const& pos);
	virtual void set(variableVector2_ptr pos);
	virtual void set(point_abstr_ptr other);
	virtual void set(point_abstr const& other);

	void operator=(glm::vec2 const& pos) { set(pos); }
	void operator=(variableVector2_ptr pos) { set(pos); }
	void operator=(point_abstr_ptr pos) { set(pos); }
	void operator=(point_abstr const& pos) { set(pos); }

	void set_constant();
	void set_tmpConstant(bool const_);

	virtual glm::vec2 at(float t) { return vec(); }

	virtual glm::vec2 closest_to_point(glm::vec2 const& pt) { return vec(); }
	virtual float dist_to_point(glm::vec2 const& pt);
	virtual float dist_to_point_2(glm::vec2 const& pt);
	virtual float dist_to_point_2(point_abstr const& pt) { return dist_to_point_2(pt.vec()); }
	virtual float dist_to_point_2(point_abstr_ptr pt) { return dist_to_point_2(*pt.get()); }
protected:
	virtual void post_set_behavior() {}
};

std::ostream& operator<<(std::ostream& os, point_abstr_ptr p);
std::ostream& operator<<(std::ostream& os, point_abstr const& p);

point_abstr_ptr operator+(point_abstr_ptr p, glm::vec2 v);
point_abstr_ptr operator-(point_abstr_ptr p, glm::vec2 v);

point_abstr_ptr operator+=(point_abstr_ptr p, glm::vec2 v);
point_abstr_ptr operator-=(point_abstr_ptr p, glm::vec2 v);

} // !geom_2d

#endif