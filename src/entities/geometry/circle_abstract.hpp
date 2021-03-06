
#ifndef CIRCLE_ABSTRACT_HPP_
#define CIRCLE_ABSTRACT_HPP_

#include "point_abstract.hpp"

#include <memory>

class circle_abstract;
using circle_abstract_ptr = std::shared_ptr<circle_abstract>;

class circle_abstract {
protected:
	point_abstract_ptr mCenter;
	variable_ptr mRadius;
public:
	circle_abstract(point_abstract_ptr center_, variable_ptr radius_, bool immovable = false);
	circle_abstract(point_abstract_ptr center_, float radius_, bool immovable = false);
	circle_abstract(glm::vec2 center_, float radius_, bool immovable = false);

	glm::vec2 center_val() const { return mCenter->pos_val(); }
	point_abstract_ptr center() const { return mCenter; }
	void set_center(point_abstract_ptr center_) { mCenter->set(center_); post_set_update(); }
	void set_center(point_abstract const& center_) { mCenter->set(center_); post_set_update(); }
	void set_center(glm::vec2 center_) { mCenter->set(glm::vec3(center_, 0.0f)); post_set_update(); }

	variable_ptr radius() const { return mRadius; }
	float radius_val() const { return mRadius->val(); }
	void set_radius(float radius_) { mRadius->set(radius_); post_set_update(); }

	glm::vec2 at(float angle);

	float dist_to_point(point_abstract_ptr pt);
	float dist_to_point(point_abstract const& pt);
	glm::vec2 closestPoint(point_abstract_ptr pt);
	glm::vec2 closestPoint(glm::vec2 pt);
protected:
	virtual void post_set_update() {}
};

#endif