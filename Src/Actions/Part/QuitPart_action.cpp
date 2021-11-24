
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

#include "QuitPart_action.hpp"

#include <Workspaces/Document.hpp>
#include <Drawables/Containers/Part.hpp>

QuitPart_action::QuitPart_action():
	mTarget(nullptr) // Target is determined when do_work is first called
{

}

bool QuitPart_action::do_work(Document* caller)
{
	mTarget = caller->target();
	if(mTarget) { // Should always be true
		mTarget->hide_origin();
	}
	return true;
}

bool QuitPart_action::undo_work(Document* caller)
{
	if(mTarget && !mTarget->has_volume()) { // Show the origin if no volume is in the part (at time of writing there is no way to add volume)
		mTarget->show_origin();
	}
	return true;
}