
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