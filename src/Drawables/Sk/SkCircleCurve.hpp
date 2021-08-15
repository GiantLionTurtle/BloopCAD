
#ifndef SKCIRCLECURVE_HPP_
#define SKCIRCLECURVE_HPP_

#include "SkGeometry.hpp"
#include "SkCurve.hpp"
#include <ConstraintsSolver/Expression.hpp>
#include <Geom/Geom2d/Circle_abstr.hpp>


class SkCircleCurve : public Geom2d::Circle_abstr, public SkCurve<std::array<ExpVec2<ExpVar>*, 1>, SkCircleCurve> {
private:
	static bool kFisrstInst;
	static glm::vec3 kColor, kColorHovered, kColorSelected; // Line color

    var_ptr mRadius;
public:
	SkCircleCurve(Geom3d::Plane_abstr* pl, bool fixed_);
	~SkCircleCurve();

	glm::vec2 center_pos() { return pos(0); }
    float radius_val() { return mRadius->eval(); }
	ExpVec2<ExpVar>* center() { return pt(0); }
    var_ptr radius() { return mRadius; }

	ExpVec2<Expression> atExp(float t);

	int selection_rank() { return 5; }
	void set_annotPos(SkSprite* sp) { sp->set(center_pos()); }
	void set_annotOffset(SkSprite* sp, int ind);
private:
	void init_impl();
	void draw_impl(Camera_ptr cam, int frame, draw_type type = draw_type::ALL);
	void update_impl();
};

#endif