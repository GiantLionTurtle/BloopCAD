
#ifndef SKLINECURVE_HPP_
#define SKLINECURVE_HPP_

#include "SkGeometry.hpp"
#include "SkCurve.hpp"
#include <constraintsSolver/expression.hpp>
#include <geometry/geometry_2d/Line_abstr.hpp>

class SkLineCurve : public Geom2d::Line_abstr, public SkCurve<std::array<Geom2d::ExpressionPoint<var_ptr>*, 2>, SkLineCurve> {
private:
	static bool kFisrstInst;
	static glm::vec3 kColor, kColorHovered, kColorSelected; // Line color

	glm::vec3 mVertices[2]; // The vertices describing the rendered line
public:
	SkLineCurve(Geom3d::plane_abstr* pl, bool fixed_);
	~SkLineCurve();

	glm::vec2 posA() { return pos(0); }
	glm::vec2 posB() { return pos(1); }
	Geom2d::ExpressionPoint<var_ptr>* ptA() { return pt(0); }
	Geom2d::ExpressionPoint<var_ptr>* ptB() { return pt(1); }

	int selection_rank() { return 3; }
	void set_annotPos(SkSprite* sp) { sp->set((posA() + posB()) / 2.0f); }
	void set_annotOffset(SkSprite* sp, int ind);
private:
	void init_impl();
	void draw_impl(Camera_ptr cam, int frame, draw_type type = draw_type::ALL);
	void update_impl();
};

#endif