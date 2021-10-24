
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
    ExpVar* radius() { return mGeom->radius(); }
	float radius_val() { return radius()->eval(); }
	void set_radius(float val) { radius()->drag(val); update(); }

	virtual void release()
	{
		center()->release();
		radius()->set_dragged(false);
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