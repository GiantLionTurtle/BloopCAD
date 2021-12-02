
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

#ifndef SKPOINT_HPP_
#define SKPOINT_HPP_

#include "SkGeometry.hpp"
#include <ConstrsSolver/Expression.hpp>
#include <Geometry/2d/Point_abstr.hpp>

#include <Graphics_utils/VertexArray.hpp>
#include <Graphics_utils/VertexBuffer.hpp>
#include <Graphics_utils/Shader.hpp>

class SkPoint : public SkPrimitiveGeometry<Geom2d::Point> {
private:
	static float kSelDist2;
	static bool kFisrstInst;
	static glm::vec3 kColor, kColorHovered, kColorSelected; // Point color
public:
	SkPoint(glm::vec2 pos, Geom3d::Plane_abstr* pl, bool fixed_);
	SkPoint(Geom2d::Point* pt, Geom3d::Plane_abstr* pl, bool fixed_);
	virtual ~SkPoint();

	Param* x() { return mGeom->x(); }
	Param* y() { return mGeom->y(); }

	void move(glm::vec2 delta);

	glm::vec2 pos() { return mGeom->pos(); }
	bool closest_2d(SelPoint& selP, glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	bool closest_2d_draggable(SkGeomDragPoint& selP, glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter);
	int selection_rank() { return 5; }
	virtual void set_annotPos(SkSprite* sp) { sp->set(pos()); }
	void set_annotOffset(SkSprite* sp, int ind);

	void append_params(std::vector<Param*>& params);

	void set(glm::vec2 pt);

	virtual void release();
protected:
	void init_impl();
	void draw_impl(Camera* cam, int frame, draw_type type = draw_type::ALL);
	void graphicUpdate_impl();
};

#endif