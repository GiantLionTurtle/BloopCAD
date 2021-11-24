
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

#include "EnterPart_action.hpp"

#include <Drawables/Containers/Part.hpp>
#include <Workspaces/Document.hpp>
#include <Bloop/Bloop.hpp>

EnterPart_action::EnterPart_action(bool set_camera):
	SwitchWorkspace_action(Bloop::PART, set_camera),
	mTarget(nullptr) // Target will be determined when do_work is called since every document has only one part.. this might change
{

}

bool EnterPart_action::do_work(Document* caller)
{
	SwitchWorkspace_action::do_work(caller);
	mTarget = caller->target(); // Get target
	if(mTarget && !mTarget->has_volume()) { // Origin planes, point and axis must be visible if no volume is present (might change to anything)
		mTarget->show_origin();
	}
	return true;
}

bool EnterPart_action::undo_work(Document* caller)
{
	SwitchWorkspace_action::undo_work(caller);
	if(mTarget) {
		mTarget->hide_origin();
	}
	return true;
}