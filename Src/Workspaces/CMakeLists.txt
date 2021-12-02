
# BloopCAD
# Copyright (C) 2020-2021 BloopCorp

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

set(Workspaces_Utils_source_files
	NavigationBar.cpp
	Part_ws.cpp
	Sketch_ws.cpp
	Home_ws.cpp
	Workspace_abstr.cpp
	EventsManager.cpp
	TestFramework_eventsManager.cpp
	MiscUI.cpp
	DocumentTree.cpp
	Document.cpp
)

add_library(Workspaces ${Workspaces_Utils_source_files})

target_link_libraries(Workspaces
	Tools
)