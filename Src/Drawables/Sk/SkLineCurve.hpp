
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

#ifndef SKLINECURVE_HPP_
#define SKLINECURVE_HPP_

#include "SkGeometry.hpp"
#include "SkCurve.hpp"
#include <ConstraintsSolver/Expression.hpp>
#include <Geometry/2d/Line_abstr.hpp>

class SkLineCurve : public Geom2d::Line_abstr, public SkCurve<std::array<ExpVec2<ExpVar>*, 2>, SkLineCurve> {
private:
	static bool kFisrstInst;
	static glm::vec3 kColor, kColorHovered, kColorSelected; // Line color

	glm::vec3 mVertices[2]; // The vertices describing the rendered line
	ExpVec2<Expression_abstr> asvecExp;
public:
	SkLineCurve(Geom3d::Plane_abstr* pl, bool fixed_);
	virtual ~SkLineCurve();

	glm::vec2 posA() { return pos(0); }
	glm::vec2 posB() { return pos(1); }
	ExpVec2<ExpVar>* ptA() { return pt(0); }
	ExpVec2<ExpVar>* ptB() { return pt(1); }
	ExpVec2<Expression_abstr> atExp(float t);
	// virtual std::vector<var_ptr> all_vars() { return { ptA()->x(), ptA()->y(), ptB()->x(), ptB()->y() }; };

	int selection_rank() { return 3; }
	void set_annotPos(SkSprite* sp) { sp->set((posA() + posB()) / 2.0f); }
	void set_annotOffset(SkSprite* sp, int ind);
// protected:
	void init_impl();
	void draw_impl(Camera* cam, int frame, draw_type type = draw_type::ALL);
	void graphicUpdate_impl();
};

#endif