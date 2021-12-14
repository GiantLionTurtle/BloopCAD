
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

#include "Dimension_tool.hpp"

#include <Drawables/Sk/SkLine.hpp>

Dimension_tool::Dimension_tool(Sketch_ws* env)
	: Tool<Sketch_ws>(env)
{

}

void Dimension_tool::init()
{
	if(mGeomA)
		mGeomA->set_hover(false);
	if(mGeomB)
		mGeomB->set_hover(false);

	mMode = Dimension_modes::NONE;
	mGeomA = nullptr;
	mGeomB = nullptr;
	mCurrHov = nullptr;

	mFilter = Geom2d::ANY;
}

bool Dimension_tool::manage_mouse_move(GdkEventMotion* event)
{
	glm::vec2 screenPos = glm::vec2(event->x, event->y);
	SkGeomDragPoint gdpt;
	SkGeometry* geom = mEnv->target()->closest_draggable(gdpt, screenPos, mEnv->state()->cam, mFilter) ? gdpt.geom : nullptr;

	// Remember this new geom (even if it is nullptr)
	// Remove hover on old hovered geometry (if it changed)
	if(geom != mCurrHov) {
		if(mCurrHov) {
			mCurrHov->set_hover(false);
		}
		if(geom) {
			geom->set_hover(true);
		}
		mCurrHov = geom;
	}
	return true;
}

bool Dimension_tool::manage_button_release(GdkEventButton* event)
{
	glm::vec2 screenPos = glm::vec2(event->x, event->y);
	SkGeomDragPoint gdpt;
	SkGeometry* geom = mEnv->target()->closest_draggable(gdpt, screenPos, mEnv->state()->cam, mFilter) ? gdpt.geom : nullptr;

	// User clicked the void
	if(!geom) {
		if(mMode != Dimension_modes::NONE) {
			create_constraint();
			init();
		}
		return true;
	}

	// User clicked on a legal (the filter is updated to match the mode) geometry
	switch(mMode) {
	case Dimension_modes::NONE:
		dispatch_mode_none(geom);
		break;
	case Dimension_modes::LINE_LEN:
		dispatch_mode_line_len(geom);
		break;
	case Dimension_modes::CIRC_RAD:
	case Dimension_modes::CIRC_DIAM:
		dispatch_mode_circle_dim(geom);
	case Dimension_modes::PTPT_HDIST:
	case Dimension_modes::PTPT_VDIST:
		dispatche_mode_pt_dist(geom);
	};

	return true;
}

void Dimension_tool::create_constraint()
{
	switch(mMode) {
	case Dimension_modes::LINE_LEN:
		DEBUG_ASSERT(mGeomA, "mGeomA is nullptr aaaaa");
		std::cout<<"Creating LINE_LEN!\n";
		// static_cast<SkLine*>(mGeomA)->geom();
		break;
	case Dimension_modes::LINEPT_DIST:
		DEBUG_ASSERT(mGeomA, "mGeomA is nullptr aaaaa");
		DEBUG_ASSERT(mGeomB, "mGeomB is nullptr bbbbb");
		std::cout<<"Creating LINEPT_DIST!\n";
		// static_cast<SkLine*>(mGeomA)->geom();
		// static_cast<SkPoint*>(mGeomB)->geom();
		break;
	case Dimension_modes::LINECIRC_DIST:
		std::cout<<"Creating LINECIRC_DIST!\n";
		DEBUG_ASSERT(mGeomA, "mGeomA is nullptr aaaaa");
		DEBUG_ASSERT(mGeomB, "mGeomB is nullptr bbbbb");
		// static_cast<SkLine*>(mGeomA)->geom();
		// static_cast<SkCircle*>(mGeomB)->geom();
		break;
	case Dimension_modes::LINELINE_DIST:
		std::cout<<"Creating LINELINE_DIST!\n";
		DEBUG_ASSERT(mGeomA, "mGeomA is nullptr aaaaa");
		DEBUG_ASSERT(mGeomB, "mGeomB is nullptr bbbbb");
		// static_cast<SkLine*>(mGeomA)->geom();
		// static_cast<SkLine*>(mGeomB)->geom();
		break;


	case Dimension_modes::PTPT_HDIST:
		std::cout<<"Creating PTPT_HDIST!\n";
		DEBUG_ASSERT(mGeomA, "mGeomA is nullptr aaaaa");
		DEBUG_ASSERT(mGeomB, "mGeomB is nullptr bbbbb");
		// static_cast<SkPoint*>(mGeomA)->geom();
		// static_cast<SkPoint*>(mGeomB)->geom();
		break;
	case Dimension_modes::PTPT_VDIST:
		std::cout<<"Creating PTPT_VDIST!\n";
		DEBUG_ASSERT(mGeomA, "mGeomA is nullptr aaaaa");
		DEBUG_ASSERT(mGeomB, "mGeomB is nullptr bbbbb");
		// static_cast<SkPoint*>(mGeomA)->geom();
		// static_cast<SkPoint*>(mGeomB)->geom();
		break;
	case Dimension_modes::PTCIRC_DIST:
		std::cout<<"Creating PTCIRC_DIST!\n";
		DEBUG_ASSERT(mGeomA, "mGeomA is nullptr aaaaa");
		DEBUG_ASSERT(mGeomB, "mGeomB is nullptr bbbbb");
		// static_cast<SkPoint*>(mGeomA)->geom();
		// static_cast<SkCircle*>(mGeomB)->geom();
		break;


	case Dimension_modes::CIRC_DIAM:
		DEBUG_ASSERT(mGeomA, "mGeomA is nullptr aaaaa");
		std::cout<<"Creating CIRC_DIAM!\n";
		// static_cast<SkCircle*>(mGeomA)->geom();
		break;
	case Dimension_modes::CIRC_RAD:
		DEBUG_ASSERT(mGeomA, "mGeomA is nullptr aaaaa");
		std::cout<<"Creating CIRC_RAD!\n";
		// static_cast<SkCircle*>(mGeomA)->geom();
		break;	

	case Dimension_modes::CIRCCIRC_DIST:
		std::cout<<"Creating CIRCCIRC_DIST!\n";
		DEBUG_ASSERT(mGeomA, "mGeomA is nullptr aaaaa");
		DEBUG_ASSERT(mGeomB, "mGeomB is nullptr bbbbb");
		// static_cast<SkCircle*>(mGeomA)->geom();
		// static_cast<SkCircle*>(mGeomB)->geom();
	}
}

void Dimension_tool::dispatch_mode_none(SkGeometry* geom)
{
	// Here geom A is not set

	if(geom->geomType() & Geom2d::LINE) {
		std::cout<<"Enter mode LINE_LEN\n";
		mGeomA = geom;
		mMode = Dimension_modes::LINE_LEN;
		mFilter = /*Geom2d::LINE | */Geom2d::POINT | Geom2d::CIRCLE;
	} else if(geom->geomType() & Geom2d::POINT) {
		std::cout<<"Enter mode PTPT_HDIST\n";
		mGeomA = geom;
		mMode = Dimension_modes::PTPT_HDIST;
		mFilter = Geom2d::LINE | Geom2d::POINT | Geom2d::CIRCLE;
	} else if(geom->geomType() & Geom2d::CIRCLE) {
		std::cout<<"Enter mode CIRC_DIAM\n";
		mGeomA = geom;
		mMode = Dimension_modes::CIRC_DIAM;
		mFilter = Geom2d::LINE | Geom2d::POINT | Geom2d::CIRCLE;
	}
}

void Dimension_tool::dispatch_mode_line_len(SkGeometry* geom)
{
	if(geom->geomType() & Geom2d::POINT) {
		std::cout<<"Enter mode LINEPT_DIST\n";
		mGeomB = geom;
		mMode = Dimension_modes::LINEPT_DIST;
		mFilter = 0;
	} else if(geom->geomType() & Geom2d::CIRCLE) {
		std::cout<<"Enter mode LINECIRC_DIST\n";
		mGeomB = geom;
		mMode = Dimension_modes::LINECIRC_DIST;
		mFilter = 0;
	} else if(geom->geomType() & Geom2d::LINE /*&& parallel check*/) {
		std::cout<<"Enter mode LINELINE_DIST\n";
		mGeomB = geom;
		mMode = Dimension_modes::LINELINE_DIST;
		mFilter = 0;
	}
}

void Dimension_tool::dispatch_mode_circle_dim(SkGeometry* geom)
{
	if(geom->geomType() & Geom2d::CIRCLE) {
		std::cout<<"Enter mode CIRCCIRC_DIST\n";
		mGeomB = geom;
		mMode = Dimension_modes::CIRCCIRC_DIST;
		mFilter = 0;
	} else if(geom->geomType() & Geom2d::LINE) {
		std::cout<<"Enter mode LINECIRC_DIST\n";
		mGeomB = geom;
		std::swap(mGeomA, mGeomB); // Line will always be first for constr creation
		mMode = Dimension_modes::LINECIRC_DIST;
		mFilter = 0;
	} else if(geom->geomType() & Geom2d::POINT) {
		std::cout<<"Enter mode PTCIRC_DIST\n";
		mGeomB = geom;
		std::swap(mGeomA, mGeomB); // Point is first for constr creation
		mMode = Dimension_modes::PTCIRC_DIST;
		mFilter = 0;
	}
}

void Dimension_tool::dispatche_mode_pt_dist(SkGeometry* geom)
{
	if(geom->geomType() & Geom2d::POINT) {
		std::cout<<"Enter mode PTPT_DIST\n";
		mGeomB = geom;
		mFilter = 0;
	} else if(geom->geomType() & Geom2d::LINE) {		
		std::cout<<"Enter mode LINEPT_DIST\n";
		mGeomB = geom;
		std::swap(mGeomA, mGeomB);
		mMode = Dimension_modes::LINEPT_DIST;
		mFilter = 0;
	} else if(geom->geomType() & Geom2d::CIRCLE) {
		std::cout<<"Enter mode PTCIRC_DIST\n";
		mGeomB = geom;
		mMode = Dimension_modes::PTCIRC_DIST;
		mFilter = 0;
	}
}