
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

#include "Constraint_tool.hpp"

#include <Actions/Common/Parallel_action.hpp>
#include <Actions/Sketch/ToggleConstraint_action.hpp>
#include <Actions/Sketch/ApplySnapshot_action.hpp>
#include <Actions/Common/ToggleBaseObject_action.hpp>
#include <Workspaces/Document.hpp>

Constraint_tool::Constraint_tool(Sketch_ws* env, int filter)
	: tool<Sketch_ws>(env)
	, mEntA(nullptr)
	, mEntB(nullptr)
	, mCurrentHover(nullptr)
	, mFilter(filter)
{
	DEBUG_ASSERT(mEnv, "No valid Workspace_abstr.");
}

void Constraint_tool::init()
{
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
	mEntA = nullptr;
	mEntB = nullptr;
	mCurrentHover = nullptr;
}
void Constraint_tool::finish()
{
	if(mEntA)
		mEntA->set_hover(false);
	if(mEntB)
		mEntB->set_hover(false);
	if(mCurrentHover)
		mCurrentHover->set_hover(false);
}

bool Constraint_tool::manage_button_press(GdkEventButton* event)
{
	glm::vec2 screenPos = glm::vec2(event->x, event->y);
	SkGeomDragPoint gdpt;
	if(!mEnv->target()->closest_draggable(gdpt, screenPos, mEnv->state()->cam, mFilter))
		return true;

	SkGeometry* geom = gdpt.geom;	
	int ent_state = could_add_geom(geom);
	if(ent_state > add_states::COULDNT_ADD) {
		add_geom(geom);
		geom->select();
		if(ent_state == add_states::WOULD_BE_COMPLETE) {
			add_constraint();
			if(mEntA)
				mEntA->unselect();
			if(mEntB)
				mEntB->unselect();
           	mEntA = nullptr;
            mEntB = nullptr;
        }
	}
	return true;
}
bool Constraint_tool::manage_mouse_move(GdkEventMotion* event)
{
	glm::vec2 screenPos = glm::vec2(event->x, event->y);
	SkGeomDragPoint gdpt;
	SkGeometry* geom = mEnv->target()->closest_draggable(gdpt, screenPos, mEnv->state()->cam, mFilter) ? gdpt.geom : nullptr;
	
	if(geom != mCurrentHover) {
		if(mCurrentHover) {
			mCurrentHover->set_hover(false);
		}
		if(could_add_geom(geom)) {
			geom->set_hover(true);
		}
		mCurrentHover = geom;
	}
	return true;
}

void Constraint_tool::add_geom(SkGeometry* geom)
{
	if(!mEntA) {
		mEntA = geom;
	} else {
		mEntB = geom;
	}
}
void Constraint_tool::add_constraint()
{
	Constraint_abstr* constr = nullptr;
	SkGeometry* priority_ent = nullptr;
	Action_ptr annotAct = nullptr;

	create_constraint(constr, priority_ent, annotAct);

	std::map<Param*, float> snp = mEnv->target()->snapshot();
	mEnv->target()->add_constraint(constr, priority_ent);
	mEnv->state()->doc->push_action(std::shared_ptr<Parallel_action>(new Parallel_action({
		std::make_shared<ApplySnapshot_action>(mEnv->target(), mEnv->target()->deltaSnapshot(snp), true),
		std::make_shared<ToggleConstraint_action>(mEnv->target(), constr, true, true)//,
		// annotAct
	})));
	// mEnv->target()->update(true);
	clear_geometries();
}

void Constraint_tool::clear_geometries()
{
	mEntA = nullptr;
	mEntB = nullptr;
}

bool Constraint_tool::is_point(SkGeometry* geom)
{
	return geom->type() & Drawable_types::POINT;
}
bool Constraint_tool::is_line(SkGeometry* geom)
{
	return geom->type() & Drawable_types::AXIS;
}
bool Constraint_tool::is_curve(SkGeometry* geom)
{
	return is_line(geom) || geom->type() & Drawable_types::CURVE;
}
bool Constraint_tool::is_point_or_curve(SkGeometry* geom)
{
	return is_point(geom) || is_curve(geom);
}