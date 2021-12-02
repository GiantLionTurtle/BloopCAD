
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

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

#ifndef TOOL_FORWARD_HPP_
#define TOOL_FORWARD_HPP_

/********* Navigation **********/
class Orbit3d_tool;
class Zoom3d_tool;
class Zoom2d_tool;
class Pan3d_tool;
class Pan2d_tool;

/********** Part design **********/
class PartDefault_tool;
class StartSketch_tool;

/********** Sketch design **********/
class SketchDefault_tool;
class Point_tool;
class Line_tool;
class Circle_tool;
class Coincidence_tool;
class Verticality_tool;
class Horizontality_tool;
class Perpendicularity_tool;


enum TOOLIDS {
	TOOLID_ORBIT,
	TOOLID_ZOOM,
	TOOLID_PAN,

	TOOLID_PARTDESIGNDEFAULT,
	TOOLID_STARTSKECTH,

	TOOLID_SKETCHDESIGNDEFAULT,
	TOOLID_POINT,
	TOOLID_LINE,
	TOOLID_CIRCLE,
	TOOLID_COINCIDENCE,
	TOOLID_VERTICALITY,
	TOOLID_HORIZONTALITY,
	TOOLID_PERPENDICULARITY
};


#endif