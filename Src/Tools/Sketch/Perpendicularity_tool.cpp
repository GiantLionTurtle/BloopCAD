
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

#include "Perpendicularity_tool.hpp"

#include <Drawables/Sk/SkLine.hpp>
#include <Actions/Sketch/ToggleConstraint_action.hpp>
#include <Workspaces/Workspace_abstr.hpp>
#include <Workspaces/Document.hpp>

Perpendicularity_tool::Perpendicularity_tool(Sketch_ws* env)
	: Constraint_tool(env)
{
	load_icon("Resources/Textures/Images/Icons/Sketch/Cursors/Perpendicularity_cursor.png");
}

int Perpendicularity_tool::could_add_geom(SkGeometry* geom)
{
	if(!geom) {
		return add_states::COULDNT_ADD;
	} else if(mEntA) {
		if(is_line(geom))
			return add_states::WOULD_BE_COMPLETE;
	} else {
		if(is_line(geom)) {
			return add_states::COULD_ADD;
		}
	}
	return add_states::COULDNT_ADD;
}
void Perpendicularity_tool::create_constraint(Constraint*& constr, SkGeometry*& priority_ent, Action_ptr& annotAct)
{
	if(!mEntA || !mEntB) {
		LOG_WARNING("Attempting to add incomplete constraint.");
		return;
	}
	std::cout<<"Add perpendicularity constraint!\n";
	constr = new LineLine_angle(static_cast<SkLine*>(mEntA)->geom(), static_cast<SkLine*>(mEntB)->geom());
	priority_ent = nullptr;

	SkConstrAnnot* annot_a = nullptr, *annot_b = nullptr;
	SkConstrAnnot::make_dual_annot(annot_a, annot_b, constr, mEnv->target()->basePlane(), 
		"Resources/Textures/Images/Icons/Sketch/Constraints/Perpendicularity.png");
	mEnv->target()->add_annot(annot_a);
	mEnv->target()->add_annot(annot_b);
	mEntA->add_annot(annot_a);
	mEntB->add_annot(annot_b);
}