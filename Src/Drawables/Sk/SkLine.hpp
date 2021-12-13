
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

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

#ifndef SKLINE_HPP_
#define SKLINE_HPP_

#include "SkCurve.hpp"
#include "SkPoint.hpp"

#include <Geometry/2d/Line_abstr.hpp>

class SkLine : public SkCurve<LinearFixed_indexer<SkPoint*, 2>, Geom2d::Line> {
private:
	static bool kFisrstInst;
	static glm::vec3 kColor, kColorHovered, kColorSelected; // Line color

	glm::vec3 mVertices[2]; // The vertices describing the rendered line
public:
	using SkCurve<LinearFixed_indexer<SkPoint*, 2>, Geom2d::Line>::handle;
	SkLine(glm::vec2 ptA_, glm::vec2 ptB_, Geom3d::Plane_abstr* pl, bool fixed_ = false);

	inline SkPoint* ptA() { return handle(0); }
	inline SkPoint* ptB() { return handle(1); }

	inline glm::vec2 posA() { return mGeom->posA(); }
	inline glm::vec2 posB() { return mGeom->posB(); }
	inline glm::dvec2 dir() { return mGeom->as_vec(); }

	int selection_rank() { return 3; }

	void set_annotPos(SkSprite* sp) { sp->set((posA() + posB()) / 2.0f); }
	void set_annotOffset(SkSprite* sp, int ind);

	void append_params(std::vector<Param*>& params);
protected:
	void init_impl();
	void draw_impl(Camera* cam, int frame, draw_type type = draw_type::ALL);
	void graphicUpdate_impl();
	void internalUpdate_impl();
};

#endif