
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

#include "EnterSketch_action.hpp"

#include <Workspaces/Document.hpp>
#include <Drawables/Containers/Sketch.hpp>
#include <Workspaces/Sketch_ws.hpp>
#include <Bloop/Bloop.hpp>

EnterSketch_action::EnterSketch_action(Sketch* target, bool set_camera)
	: SwitchWorkspace_action(Bloop::workspace_types::SKETCH, set_camera)
	, mTarget(target)
{

}

bool EnterSketch_action::do_work(Document* caller)
{
	caller->window()->sketchWorkspace()->set_target(mTarget); // The Sketch workspace now edits the target sketch
	SwitchWorkspace_action::do_work(caller); // Do switch the workspace
	return true;
}