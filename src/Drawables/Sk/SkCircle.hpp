
#ifndef SKCIRCLE_HPP_
#define SKCIRCLE_HPP_

#include "SkHandleCurve.hpp"
#include "SkCircleCurve.hpp"

class SkCircle : public SkHandleCurve<SkHandleCurve_FixedIndexer<1, SkCircleCurve>> {
public:
	SkCircle(glm::vec2 center_, float radius_, Geom3d::plane_abstr* pl, bool fixed_):
		SkHandleCurve({ center_ }, pl, fixed_)
	{
		set_name("SkLine");
		set_radius(radius_);
	}


	SkPoint* center() { return handle(0); }
	glm::vec2 center_pos() { return handle(0)->pos(); }
	void set_center(glm::vec2 pos) { center()->set(pos); }
    var_ptr radius() { return curve()->radius(); }
	float radius_val() { return radius()->eval(); }
	void set_radius(float val) { radius()->set(val); }

	int selection_rank() { return 3; }
};

#endif