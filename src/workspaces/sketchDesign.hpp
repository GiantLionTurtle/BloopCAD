
#ifndef SKETCHDESIGN_HPP_
#define SKETCHDESIGN_HPP_

#include "workspace.hpp"
#include <entities/sketch.hpp>
#include <tools/tools_forward.hpp>

#include <map>

/*
	@class sketchDesign describes the workspace for parametrized sketches
	@parent : workspace
*/
class sketchDesign : public workspace {
protected:
	std::shared_ptr<pan2d_tool> 				mPan_tool;
	std::shared_ptr<sketchDesignDefault_tool> 	mSketchDesignDefault_tool;
	std::shared_ptr<point_tool> 				mPoint_tool;
	std::shared_ptr<line_tool> 					mLine_tool;
	std::shared_ptr<circle_tool> 				mCircle_tool;
	std::shared_ptr<coincidence_tool> 			mCoincidence_tool;
	std::shared_ptr<verticality_tool> 			mVerticality_tool;
	std::shared_ptr<horizontality_tool> 		mHorizontality_tool;
	std::shared_ptr<perpendicularity_tool> 		mPerpendiculatiry_tool;

	std::map<std::string, std::pair<Gtk::Button*, Gtk::Image*>> mButtons; // Buttons with names and icons
	sketch_ptr mTarget;
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

	void set_target(sketch_ptr sk) { mTarget = sk; }
	sketch_ptr target() { return mTarget; }

	virtual bool set_tool(int name);

	bool manage_key_press(GdkEventKey* event);

	std::shared_ptr<pan2d_tool> pan() { return mPan_tool; }
	std::shared_ptr<sketchDesignDefault_tool> sketchDesignDefault() { return mSketchDesignDefault_tool; }
	std::shared_ptr<point_tool> point() { return mPoint_tool; }
	std::shared_ptr<line_tool> line() { return mLine_tool; }
	std::shared_ptr<circle_tool> circle() { return mCircle_tool; }
	std::shared_ptr<coincidence_tool> coincidence() { return mCoincidence_tool; }
	std::shared_ptr<verticality_tool> verticality() { return mVerticality_tool; }
	std::shared_ptr<horizontality_tool> horizontality() { return mHorizontality_tool; }
	std::shared_ptr<perpendicularity_tool> perpendicularity() { return mPerpendiculatiry_tool; }

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