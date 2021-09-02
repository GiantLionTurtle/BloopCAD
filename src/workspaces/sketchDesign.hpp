
#ifndef SKETCHDESIGN_HPP_
#define SKETCHDESIGN_HPP_

#include "workspace.hpp"
#include <Drawables/Sketch.hpp>
#include <Tools/Tools_forward.hpp>

#include <map>

/*
	@class sketchDesign describes the workspace for parametrized sketches
	@parent : workspace
*/
class sketchDesign : public workspace {
protected:
	std::shared_ptr<Pan2d_tool> 				mPan3d_tool;
	std::shared_ptr<Zoom2d_tool> 				mZoom3d_tool;
	std::shared_ptr<SketchDefault_tool> 		mSketchDefault_tool;
	std::shared_ptr<Point_tool> 				mPoint_tool;
	std::shared_ptr<Line_tool> 					mLine_tool;
	std::shared_ptr<Circle_tool> 				mCircle_tool;
	std::shared_ptr<Coincidence_tool> 			mCoincidence_tool;
	std::shared_ptr<Verticality_tool> 			mVerticality_tool;
	std::shared_ptr<Horizontality_tool> 		mHorizontality_tool;
	std::shared_ptr<Perpendicularity_tool> 		mPerpendiculatiry_tool;

	std::map<std::string, std::pair<Gtk::Button*, Gtk::Image*>> mButtons; // Buttons with names and icons
	Sketch* mTarget;
public:
	/*
		@function sketchDesign creates an empty workspace

		@param parent : The parent window of the workspace

		@note : Not recommended since it won't have buttons
	*/
	sketchDesign(bloop* parent);
	/*
		@function sketchDesign creates an empty workspace

		@param builder : 	The gtk builder that has all the info from an xml file
		@param parent : 	The parent window of the workspace	
	*/
	sketchDesign(Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent);
	~sketchDesign();

	void set_target(Sketch* sk) { mTarget = sk; }
	Sketch* target() { return mTarget; }

	virtual bool set_tool(int name);

	bool manage_key_press(GdkEventKey* event);
	bool manage_mouse_scroll(GdkEventScroll* event);

	std::shared_ptr<Pan2d_tool> pan() { return mPan3d_tool; }
	std::shared_ptr<SketchDefault_tool> sketchDesignDefault() { return mSketchDefault_tool; }
	std::shared_ptr<Point_tool> point() { return mPoint_tool; }
	std::shared_ptr<Line_tool> line() { return mLine_tool; }
	// std::shared_ptr<Circle_tool> circle() { return mCircle_tool; }
	std::shared_ptr<Coincidence_tool> coincidence() { return mCoincidence_tool; }
	std::shared_ptr<Verticality_tool> verticality() { return mVerticality_tool; }
	std::shared_ptr<Horizontality_tool> horizontality() { return mHorizontality_tool; }
	std::shared_ptr<Perpendicularity_tool> perpendicularity() { return mPerpendiculatiry_tool; }

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