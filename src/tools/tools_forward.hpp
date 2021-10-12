
#ifndef TOOL_FORWARD_HPP_
#define TOOL_FORWARD_HPP_

/********* Navigation **********/
class orbit_tool;
class zoom_tool;
class zoom2d_tool;
class pan_tool;
class pan2d_tool;

/********** Part design **********/
class partDesignDefault_tool;
class startSketch_tool;

/********** Sketch design **********/
class sketchDesignDefault_tool;
class point_tool;
class line_tool;
class circle_tool;
class coincidence_tool;
class verticality_tool;
class horizontality_tool;
class perpendicularity_tool;


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