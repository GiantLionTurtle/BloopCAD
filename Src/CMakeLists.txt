
# BloopCAD
# Copyright (C) 2020  BloopCorp

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

add_subdirectory(Bloop)
add_subdirectory(Utils)
add_subdirectory(Graphics_utils)
add_subdirectory(Geometry)
add_subdirectory(Drawables)
add_subdirectory(Actions)
add_subdirectory(Tools)
add_subdirectory(Workspaces)
add_subdirectory(ConstrsSolver)

add_executable(BloopCAD
	Main.cpp
)

target_link_libraries(BloopCAD
	Vendor
	Utils
	Graphics_utils
	Drawables
	Tools
	Workspaces
	Bloop
	${GTKMM_LIBRARIES}
	${CMAKE_DL_LIBS} epoxy
)

