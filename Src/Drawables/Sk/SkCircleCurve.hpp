
// BloopCAD
// Copyright (C) 2020  BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SKCIRCLECURVE_HPP_
#define SKCIRCLECURVE_HPP_

#include "SkGeometry.hpp"
#include "SkCurve.hpp"
#include <ConstraintsSolver/Expression.hpp>
#include <Geometry/2d/Circle_abstr.hpp>


class SkCircleCurve : public Geom2d::Circle_abstr, public SkCurve<std::array<ExpVec2<ExpVar>*, 1>, SkCircleCurve> {
private:
	static bool kFisrstInst;
	static glm::vec3 kColor, kColorHovered, kColorSelected; // Line color

    var_ptr mRadius;
public:
	SkCircleCurve(Geom3d::Plane_abstr* pl, bool fixed_);
	virtual ~SkCircleCurve();

	glm::vec2 center_pos() { return pos(0); }
    float radius_val() { return mRadius->eval(); }
	ExpVec2<ExpVar>* center() { return pt(0); }
    var_ptr radius() { return mRadius; }

	ExpVec2<Expression_abstr> atExp(float t);
	
	virtual void release()
	{
		center()->x()->set_dragged(false);
		center()->y()->set_dragged(false);
		radius()->set_dragged(false);
	}
	virtual std::vector<var_ptr> all_vars()
	{
		return { center()->x(), center()->y(), radius() };
	}

	int selection_rank() { return 5; }
	void set_annotPos(SkSprite* sp) { sp->set(center_pos()); }
	void set_annotOffset(SkSprite* sp, int ind);
private:
	void init_impl();
	void draw_impl(Camera* cam, int frame, draw_type type = draw_type::ALL);
	void graphicUpdate_impl();
};

#endif