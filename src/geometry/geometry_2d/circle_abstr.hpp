
#ifndef CIRCLE_ABSTR2_HPP_
#define CIRCLE_ABSTR2_HPP_

#include "point_abstr.hpp"

#include <memory>

namespace geom_2d {

class circle_abstr : public geometry_2d_abstr {
protected:
	point_abstr_ptr mCenter;
	variable_ptr mRadius;
public:
	circle_abstr(point_abstr_ptr center_, variable_ptr radius_, bool immovable = false);
	circle_abstr(point_abstr_ptr center_, float radius_, bool immovable = false);
	circle_abstr(glm::vec2 center_, float radius_, bool immovable = false);
	virtual ~circle_abstr();

	glm::vec2 center_vec() const { return mCenter->vec(); }
	point_abstr_ptr center() const { return mCenter; }
	void set_center(point_abstr_ptr center_) { mCenter->set(center_); post_set_behavior(); }
	void set_center(point_abstr const& center_) { mCenter->set(center_); post_set_behavior(); }
	void set_center(glm::vec2 center_) { mCenter->set(glm::vec2(center_)); post_set_behavior(); }

	variable_ptr radius() const { return mRadius; }
	float radius_val() const { return mRadius->val(); }
	void set_radius(float radius_) { mRadius->set(radius_); post_set_behavior(); }

	virtual glm::vec2 at(float t);

	virtual glm::vec2 closest_to_point(glm::vec2 const& pt);
	virtual float dist_to_point(glm::vec2 const& pt);
protected:
	virtual void post_set_behavior() {}
};

} // !geom_2d

#endif