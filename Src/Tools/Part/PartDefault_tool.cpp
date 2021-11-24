
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

#include "PartDefault_tool.hpp"

#include <Utils/Debug_util.hpp>
#include <Workspaces/Document.hpp>

PartDefault_tool::PartDefault_tool(Part_ws* env):
	SimpleSelector_tool(env)
{
	if(!mEnv) {
		LOG_ERROR("Null environnement in tool.");
	}
}

void PartDefault_tool::act_on_entity(Drawable* drw)
{
	drw->invoke_workspace(mEnv->state()->doc);
}