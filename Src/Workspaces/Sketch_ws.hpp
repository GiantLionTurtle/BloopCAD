
#ifndef SKETCHDESIGN_HPP_
#define SKETCHDESIGN_HPP_

#include "Workspace_abstr.hpp"
#include <Drawables/Containers/Sketch.hpp>
#include <Tools/Tools_forward.hpp>

#include <map>

/*
	@class Sketch_ws describes the workspace for parametrized sketches
	@parent : workspace
*/
class Sketch_ws : public Workspace_abstr {
protected:
	Pan2d_tool* 				mPan3d_tool;
	Zoom2d_tool* 				mZoom3d_tool;
	SketchDefault_tool* 		mSketchDefault_tool;
	Point_tool* 				mPoint_tool;
	Line_tool* 					mLine_tool;
	Circle_tool* 				mCircle_tool;
	Coincidence_tool* 			mCoincidence_tool;
	Verticality_tool* 			mVerticality_tool;
	Horizontality_tool* 		mHorizontality_tool;
	Perpendicularity_tool* 		mPerpendiculatiry_tool;

	std::map<std::string, std::pair<Gtk::Button*, Gtk::Image*>> mButtons; // Buttons with names and icons
	Sketch* mTarget;
public:
	/*
		@function Sketch_ws creates an empty workspace

		@param builder : 	The gtk builder that has all the info from an xml file
		@param parent : 	The parent window of the workspace	
	*/
	Sketch_ws(Glib::RefPtr<Gtk::Builder> const& builder, Bloop* parent);
	~Sketch_ws();

	void set_target(Sketch* sk) { mTarget = sk; }
	Sketch* target() { return mTarget; }

	virtual bool set_tool(int name);

	bool manage_key_press(GdkEventKey* event);
	bool manage_mouse_scroll(GdkEventScroll* event);

	Pan2d_tool* pan() { return mPan3d_tool; }
	SketchDefault_tool* default_tool() { return mSketchDefault_tool; }
	Point_tool* point() { return mPoint_tool; }
	Line_tool* line() { return mLine_tool; }
	// Circle_tool* circle() { return mCircle_tool; }
	Coincidence_tool* coincidence() { return mCoincidence_tool; }
	Verticality_tool* verticality() { return mVerticality_tool; }
	Horizontality_tool* horizontality() { return mHorizontality_tool; }
	Perpendicularity_tool* perpendicularity() { return mPerpendiculatiry_tool; }

	/*
		@function line invokes the line tool
	*/
	void begin_line();
	/*
		@function line invokes the line tool
	*/
	void begin_point();
	/*
		@function rectangle invokes the rectangle tool

		@note : This tool doesn't exist yet
	*/
	void begin_rectangle();
	/*
		@function polygon invokes the polygon tool

		@note : This tool doesn't exist yet
	*/
	void begin_polygon();
	/*
		@function circle invokes the circle tool

		@note : This tool doesn't exist yet
	*/
	void begin_circle();
	/*
		@function threePointsArc invokes the threePointsArc tool

		@note : This tool doesn't exist yet
	*/
	void begin_threePointsArc();

	/*
		@function dimension invokes the dimension tool

		@note : This tool doesn't exist yet
	*/
	void begin_dimension();
	/*
		@function verticality invokes the verticality tool

		@note : This tool doesn't exist yet
	*/
	void begin_verticality();
	/*
		@function horizontality invokes the horizontality tool

		@note : This tool doesn't exist yet
	*/
	void begin_horizontality();
	/*
		@function perpendicularity invokes the perpendicularity tool

		@note : This tool doesn't exist yet
	*/
	void begin_perpendicularity();
	/*
		@function parallelism invokes the parallelism tool

		@note : This tool doesn't exist yet
	*/
	void begin_parallelism();
	/*
		@function coincidence invokes the coincidence tool

		@note : This tool doesn't exist yet
	*/
	void begin_coincidence();
	/*
		@function equality invokes the equality tool

		@note : This tool doesn't exist yet
	*/
	void begin_equality();

	/*
		@function to_svg saves the sketch as an svg file
	*/
	void to_svg();

	/*
		@function finish exits the sketch workspace
	*/
	void finish();

	virtual std::string name() { return "sketch design"; }
};

#endif