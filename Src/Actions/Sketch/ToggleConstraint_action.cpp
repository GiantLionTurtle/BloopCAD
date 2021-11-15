
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

#include "ToggleConstraint_action.hpp"

#include <Utils/Debug_util.hpp>
#include <Workspaces/Document.hpp>

ToggleConstraint_action::ToggleConstraint_action(Sketch* parent_sketch, Constraint* constr, bool toggle_to, bool done_at_creation)
	: mSketch_parent(parent_sketch)
	, mConstraint(constr)
	, mToggle_to(toggle_to)
	, mDone(done_at_creation)
{
	DEBUG_ASSERT(parent_sketch, "Invalid sketch.");
	DEBUG_ASSERT(constr, "Invalid constraint.");
}

bool ToggleConstraint_action::do_work(Document* caller)
{
	if(mDone)
		return true;
	mSketch_parent->toggle_constraint(mConstraint, mToggle_to); // Let sketch handle everything..
	return true;
}
bool ToggleConstraint_action::undo_work(Document* caller)
{
	mDone = false; // Will never be true again
	mSketch_parent->toggle_constraint(mConstraint, !mToggle_to);
	return true;
}