
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

#include "Verticality_tool.hpp"

#include <Drawables/Sk/SkLine.hpp>
#include <Workspaces/Workspace_abstr.hpp>
#include <Actions/Sketch/ToggleConstraint_action.hpp>
#include <Workspaces/Document.hpp>

Verticality_tool::Verticality_tool(Sketch_ws* env)
	: Constraint_tool(env)
{
	load_icon("Resources/Textures/Images/Icons/Sketch/Cursors/Verticality_cursor.png");
}

int Verticality_tool::could_add_geom(SkGeometry* geom)
{
	if(!geom) {
		return add_states::COULDNT_ADD;
	} else if(mEntA) {
		if(is_point(geom))
			return add_states::WOULD_BE_COMPLETE;
	} else {
		if(is_point(geom)) {
			return add_states::COULD_ADD;
		} else if(is_line(geom)) {
			return add_states::WOULD_BE_COMPLETE;
		}
	}
	return add_states::COULDNT_ADD;
}

void Verticality_tool::create_constraint(Constraint*& constr, SkGeometry*& priority_ent, Action_ptr& annotAct)
{
	if(!mEntA && !mEntB) {
		LOG_WARNING("Attempting to add incomplete constraint.");
		return;
	}

	// When this is enabled, check if the dimension tool behaves correctly 
	// for vertical lines traced up-down and down-up

	if(!mEntB) {
		// constr = new SkLine_verticality(mEnv->target()->basePlane(), static_cast<SkLineCurve*>(mEntA));
	} else {
		// constr = new SkPointPoint_verticality(mEnv->target()->basePlane(), static_cast<SkPoint*>(mEntA), static_cast<SkPoint*>(mEntB));
		priority_ent = mEntB;
	}
}