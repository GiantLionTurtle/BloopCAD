
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
#include <Drawables/Sk/SkCircle.hpp>

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
	// reset preview (delete!!!!!)

	mFilter = Geom2d::ANY;

	std::cout<<"Blank dimension tool\n";
}
void Dimension_tool::finish()
{
	init();
}

bool Dimension_tool::manage_key_press(GdkEventKey* event)
{
	if(event->keyval == GDK_KEY_Escape && mMode != Dimension_modes::NONE) {
		finish();
		return false;
	}
	return true;
}

bool Dimension_tool::manage_mouse_move(GdkEventMotion* event)
{
	glm::vec2 screenPos = glm::vec2(event->x, event->y);
	glm::vec2 plPos = mEnv->state()->cam->screen_to_plane(screenPos, mEnv->target()->basePlane());
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

	if(mMode == Dimension_modes::NONE)
		return true;
	
	switch(mMode) {
	case Dimension_modes::PTPT_HDIST:
	case Dimension_modes::PTPT_VDIST:
		if(mGeomB != nullptr) {
			int tmp_mode = mode_selection_ptpt_dist(plPos);

			if(tmp_mode != mMode) {
				mMode = tmp_mode;
				std::cout<<"Switched mode to PTPT_"
				<<(mMode == Dimension_modes::PTPT_HDIST ? "H" : "V")<<"DIST\n";
			}
		}
		break;
	case Dimension_modes::CIRC_DIAM:
	case Dimension_modes::CIRC_RAD:
		int tmp_mode = mode_selection_circ_dim(plPos);

		if(tmp_mode != mMode) {
			mMode = tmp_mode;
			std::cout<<"Enter mode CIRC_"<<(mMode == Dimension_modes::CIRC_DIAM ? "DIAM" : "RAD")<<"\n";
		}
	}

	// if(mPreview)
		// mPreview->move(plPos);

	return true;
}

bool Dimension_tool::manage_button_release(GdkEventButton* event)
{
	glm::vec2 screenPos = glm::vec2(event->x, event->y);
	glm::vec2 plPos = mEnv->state()->cam->screen_to_plane(screenPos, mEnv->target()->basePlane());
	SkGeomDragPoint gdpt;
	SkGeometry* geom = mEnv->target()->closest_draggable(gdpt, screenPos, mEnv->state()->cam, mFilter) ? gdpt.geom : nullptr;

	// User clicked the void
	if(!geom) {
		if(mMode != Dimension_modes::NONE) {
			create_constraint();
			finish();
		}
		return true;
	}

	// User clicked on a legal (the filter is updated to match the mode) geometry
	switch(mMode) {
	case Dimension_modes::NONE:
		dispatch_mode_none(geom, plPos);
		break;
	case Dimension_modes::LINE_LEN:
		dispatch_mode_line_len(geom);
		break;
	case Dimension_modes::CIRC_RAD:
	case Dimension_modes::CIRC_DIAM:
		dispatch_mode_circle_dim(geom);
	case Dimension_modes::PTPT_HDIST:
	case Dimension_modes::PTPT_VDIST:
		dispatche_mode_pt_dist(geom, plPos);
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

void Dimension_tool::dispatch_mode_none(SkGeometry* geom, glm::vec2 plPos)
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
		mGeomA = geom;
		mMode = mode_selection_circ_dim(plPos);
		mFilter = Geom2d::LINE | Geom2d::POINT | Geom2d::CIRCLE;
		std::cout<<"Enter mode CIRC_"<<(mMode == Dimension_modes::CIRC_DIAM ? "DIAM" : "RAD")<<"\n";
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

void Dimension_tool::dispatche_mode_pt_dist(SkGeometry* geom, glm::vec2 plPos)
{
	if(geom->geomType() & Geom2d::POINT) {
		mGeomB = geom;
		mMode = mode_selection_ptpt_dist(plPos);
		mFilter = 0;
		std::cout<<"Enter mode PTPT_"<<(mMode == Dimension_modes::PTPT_HDIST ? "H" : "V")<<"DIST\n";
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

int Dimension_tool::mode_selection_ptpt_dist(glm::vec2 plPos)
{
	glm::vec2 pA = static_cast<SkPoint*>(mGeomA)->geom()->pos();
	glm::vec2 pB = static_cast<SkPoint*>(mGeomB)->geom()->pos();

	// Extreme positions of both points
	double max_x = std::max(pA.x, pB.x);
	double max_y = std::max(pA.y, pB.y);
	double min_x = std::min(pA.x, pB.x);
	double min_y = std::min(pA.y, pB.y);

	// where the mouse is in relation to the points
	// x_mid => x in the middle (not actual middle but between extremes)
	bool x_mid = plPos.x >= min_x && plPos.x <= max_x;
	bool y_mid = plPos.y >= min_y && plPos.y <= max_y;

	// both distances between points to make decision 
	// the largest distance will dictate the default
	// orientation of the distance
	double dist_h = max_x - min_x;
	double dist_v = max_y - min_y;

	if(x_mid && y_mid) { // Mouse is between points, default
		return dist_h > dist_v ? 
			Dimension_modes::PTPT_HDIST : Dimension_modes::PTPT_VDIST;
	} else if(!x_mid && !y_mid) { // Mouse is outside bounds of points, default
		return dist_h > dist_v ?
			Dimension_modes::PTPT_HDIST : Dimension_modes::PTPT_VDIST;
	} else if(!x_mid) { // On the right or the left
		return Dimension_modes::PTPT_VDIST;
	} else if(!y_mid) { // Above or below
		return Dimension_modes::PTPT_HDIST;
	}

	BLOOP_MARKER;
	return dist_h > dist_v ? 
			Dimension_modes::PTPT_HDIST : Dimension_modes::PTPT_VDIST; 
}
int Dimension_tool::mode_selection_circ_dim(glm::vec2 plPos)
{
	SkCircle* circ = static_cast<SkCircle*>(mGeomA);
	
	if(glm::distance(glm::dvec2(plPos), circ->geom()->center_pos()) < circ->radius_val()) {
		return Dimension_modes::CIRC_DIAM;
	} else {
		return Dimension_modes::CIRC_RAD;
	}
}