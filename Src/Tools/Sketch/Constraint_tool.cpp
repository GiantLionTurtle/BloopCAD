
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
#include <Workspaces/Document.hpp>

Constraint_tool::Constraint_tool(Sketch_ws* env, int filter):
	tool<Sketch_ws>(env),
	mEntA(nullptr),
	mEntB(nullptr),
	mCurrentHover(nullptr),
	mFilter(filter)
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
	SkDrawable* ent = mEnv->target()->closest_draggable(screenPos, mEnv->state()->cam, mFilter).ent;
	
	int ent_state = could_add_geom(ent);
	if(ent_state > add_states::COULDNT_ADD) {
		add_geom(ent);
		ent->select();
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
	auto draggablePt = mEnv->target()->closest_draggable(screenPos, mEnv->state()->cam, mFilter);
	SkDrawable* ent = draggablePt ? draggablePt.ent : nullptr;

	if(ent != mCurrentHover) {
		if(mCurrentHover) {
			mCurrentHover->set_hover(false);
		}
		if(could_add_geom(ent)) {
			ent->set_hover(true);
		}
		mCurrentHover = ent;
	}
	return true;
}

void Constraint_tool::add_geom(SkDrawable* geom)
{
	if(!mEntA) {
		mEntA = geom;
	} else {
		mEntB = geom;
	}
}
void Constraint_tool::add_constraint()
{
	SkConstraint* constr = nullptr;
	SkDrawable* priority_ent = nullptr;
	
	create_constraint(constr, priority_ent);

	std::map<var_ptr, float> snp = mEnv->target()->snapshot();
	mEnv->target()->add_constraint(constr, priority_ent);
	mEnv->state()->doc->push_action(std::shared_ptr<Parallel_action>(new Parallel_action({
		std::make_shared<ApplySnapshot_action>(mEnv->target(), mEnv->target()->deltaSnapshot(snp), true),
		std::make_shared<ToggleConstraint_action>(mEnv->target(), constr, true, true)
	})));
	// mEnv->target()->update(true);
	clear_geometries();
}

void Constraint_tool::clear_geometries()
{
	mEntA = nullptr;
	mEntB = nullptr;
}

bool Constraint_tool::is_point(SkDrawable* drw)
{
	return drw->type() & Drawable_types::POINT;
}
bool Constraint_tool::is_line(SkDrawable* drw)
{
	return drw->type() & Drawable_types::AXIS;
}
bool Constraint_tool::is_curve(SkDrawable* drw)
{
	return is_line(drw) || drw->type() & Drawable_types::CURVE;
}
bool Constraint_tool::is_point_or_curve(SkDrawable* drw)
{
	return is_point(drw) || is_curve(drw);
}