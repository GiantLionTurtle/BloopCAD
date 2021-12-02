
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

set(Actions_source_files
	
	# Actions that act within the Sketch workspace
	Sketch/EnterSketch_action.cpp
	Sketch/ToggleConstraint_action.cpp
	Sketch/ApplySnapshot_action.cpp
	
	# Actions that act within the Part workspace
	Part/EnterPart_action.cpp
	Part/QuitPart_action.cpp

	# Actions that do not act within a specific workspace
	Common/SwitchWorkspace_action.cpp
	Common/MoveCamera_action.cpp
	Common/ToggleBaseObject_action.cpp
	Common/Serial_action.cpp
	Common/Parallel_action.cpp
)

add_library(Actions ${Actions_source_files})

target_link_libraries(Actions
	Utils
)