
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

set(Drawables_source_files
	Base/Drawable.cpp
	Containers/Part.cpp
	Containers/Sketch.cpp
	Containers/Folder.cpp
	
	Sk/SkLine.cpp
	Sk/SkCircle.cpp
	Sk/SkPoint.cpp
	Sk/SkSelRect.cpp
	Sk/SkSprite.cpp
	Sk/SkNumericAnnot.cpp
	Sk/SkLengthAnnot.cpp

	3d/Plane.cpp
	3d/Point.cpp
)

add_library(Drawables ${Drawables_source_files})

target_link_libraries(Drawables
	Geom
	Graphics_utils
	ConstrsSolver
)