
#ifndef SKCIRCLE_HPP_
#define SKCIRCLE_HPP_


#include "SkCurve.hpp"
#include <Geometry/2d/Circle_abstr.hpp>

class SkCircle : public SkCurve<LinearFixed_indexer<SkPoint*, 1>, Geom2d::Circle> {
private:
	static bool kFisrstInst;
	static glm::vec3 kColor, kColorHovered, kColorSelected; // Line color
public:
	SkCircle(glm::vec2 center_, float radius_, Geom3d::Plane_abstr* pl, bool fixed_);

	SkPoint* center() { return handle(0); }
	glm::vec2 center_pos() { return handle(0)->pos(); }
	void set_center(glm::vec2 pos) { center()->set(pos); }
    Param* radius() { return mGeom->radius(); }
	float radius_val() { return radius()->val(); }
	void set_radius(float val) { radius()->drag(val); update(); }

	virtual void release()
	{
		center()->release();
		radius()->set_frozen(false);
	}

	int selection_rank() { return 5; }

	void set_annotPos(SkSprite* sp) { sp->set(center_pos()); }
	void set_annotOffset(SkSprite* sp, int ind);
protected:
	void init_impl();
	void draw_impl(Camera* cam, int frame, draw_type type = draw_type::ALL);
	void graphicUpdate_impl();
};

#endif