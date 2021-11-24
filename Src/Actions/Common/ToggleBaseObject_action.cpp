
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

#include "ToggleBaseObject_action.hpp"

#include <Utils/Debug_util.hpp>
#include <Workspaces/Document.hpp>
#include <Bloop/BaseObject.hpp>

ToggleBaseObject_action::ToggleBaseObject_action(BaseObject* obj, bool toggle_to):
	mObj(obj),
	mToggle_to(toggle_to)
{
	DEBUG_ASSERT(obj, "Invalid object.");
}

bool ToggleBaseObject_action::do_work(Document* caller)
{
	mObj->set_exists(mToggle_to); // Notify the object of it's faith
	return true;
}
bool ToggleBaseObject_action::undo_work(Document* caller)
{
	mObj->set_exists(!mToggle_to); // Notify the object of it's faith
	return true;
}