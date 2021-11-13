
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

#ifndef SKETCHDRAWABLE_HPP_
#define SKETCHDRAWABLE_HPP_

#include <Drawables/Base/Drawable.hpp>
#include <Geometry/3d/Plane_abstr.hpp>
#include <ConstraintsSolver/Expression.hpp>

class SkIntDrawable;
class SkGeometry;

struct SkGeomDragPoint {
	SkGeomDragPoint()
		: geom(nullptr)
	{

	}
	SkGeomDragPoint(SkGeometry* dr)
		: geom(dr)
	{
		
	}

	operator bool() { return geom; }

	SkGeometry* geom;
};

class SkDrawable : virtual public Drawable {
public:
	enum SkNotifiers { SET = 5 };
protected:
	Geom3d::Plane_abstr* mBasePlane; // Should it be the parent sketch directly??
public:
	SkDrawable(Geom3d::Plane_abstr* pl)
		: mBasePlane(pl)
	{
		mType |= Drawable_types::SKDRAWABLE;
	}

	Geom3d::Plane_abstr* basePlane() { return mBasePlane; }
	void set_basePlane(Geom3d::Plane_abstr* pl) { mBasePlane = pl; update(); }
};

// interactive drawables
class SkIntDrawable : public SkDrawable {
public:
	SkIntDrawable(Geom3d::Plane_abstr* pl)
		: SkDrawable(pl)
	{
		
	}
	virtual bool closest_2d(SelPoint& selP, glm::vec2 planePos, Camera* cam, glm::vec2 cursorPos, int filter) = 0;
	virtual void move_selected(glm::vec2 delta) { if(selected()) move(delta); }
	virtual void move(glm::vec2 delta) {}
	virtual void release() {}
};

#endif