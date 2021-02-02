
#ifndef POINT_ABSTR3_HPP_
#define POINT_ABSTR3_HPP_

#include <constraintsSolver/expressionVector3.hpp>
#include "geometry_3d_abstr.hpp"

#include <glm/glm.hpp>

namespace geom_3d {

class point_abstr : public geometry_3d_abstr {
protected:
	variableVector3_ptr mPos;
public:
	point_abstr(glm::vec3 pos, bool immovable = false);
	point_abstr(variableVector3_ptr pos, bool immovable = false);
    virtual ~point_abstr();

	virtual glm::vec3 vec() const { return mPos->get(); }
	// virtual glm::vec3 vec_world() const { return mBasePlane->point_2d_to_3d(mPos->get()); }
	virtual variableVector3_ptr var() const { return mPos; }

	virtual void set(glm::vec3 const& pos);
	virtual void set(variableVector3_ptr pos);
	virtual void set(point_abstr_ptr other);
	virtual void set(point_abstr const& other);

	void operator=(glm::vec3 const& pos) { set(pos); }
	void operator=(variableVector3_ptr pos) { set(pos); }
	void operator=(point_abstr_ptr pos) { set(pos); }
	void operator=(point_abstr const& pos) { set(pos); }

	void set_constant();
	void set_tmpConstant(bool const_);

	virtual glm::vec3 closest_to_point(glm::vec3 const& pt) { return vec(); }
	virtual float dist_to_point(glm::vec3 const& pt);
	virtual float dist_to_point_2(glm::vec3 const& pt);
	virtual float dist_to_point_2(point_abstr const& pt) { return dist_to_point_2(pt.vec()); }
	virtual float dist_to_point_2(point_abstr_ptr pt) { return dist_to_point_2(*pt.get()); }
protected:
	virtual void post_set_behavior() {}
};

std::ostream& operator<<(std::ostream& os, point_abstr_ptr p);
std::ostream& operator<<(std::ostream& os, point_abstr const& p);

point_abstr_ptr operator+(point_abstr_ptr p, glm::vec3 v);
point_abstr_ptr operator-(point_abstr_ptr p, glm::vec3 v);

} // !geom_3d

#endif