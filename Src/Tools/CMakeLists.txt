
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

set(Tools_source_files
	Tool.cpp
	
	Navigation3d/Orbit3d_tool.cpp
	Navigation3d/Pan3d_tool.cpp
	Navigation3d/Zoom3d_tool.cpp

	Part/PartDefault_tool.cpp
	Part/StartSketch_tool.cpp

	Sketch/Pan2d_tool.cpp
	Sketch/Zoom2d_tool.cpp
	Sketch/SketchDefault_tool.cpp
	Sketch/Point_tool.cpp
	Sketch/Line_tool.cpp
	Sketch/Circle_tool.cpp
	Sketch/Constraint_tool.cpp
	Sketch/Coincidence_tool.cpp
	Sketch/Perpendicularity_tool.cpp
	Sketch/Verticality_tool.cpp
	Sketch/Horizontality_tool.cpp
)

add_library(Tools ${Tools_source_files})

target_link_libraries(Tools
	Drawables
	Actions
)