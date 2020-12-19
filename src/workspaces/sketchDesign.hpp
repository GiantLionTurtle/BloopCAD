
#ifndef SKETCHDESIGN_HPP_
#define SKETCHDESIGN_HPP_

#include "workspace.hpp"

#include <map>

/*
	@class sketchDesign describes the workspace for parametrized sketches
	@parent : workspace
*/
class sketchDesign : public workspace {
private:
	std::map<std::string, std::pair<Gtk::Button*, Gtk::Image*>> mButtons; // Buttons with names and icons
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

	/*
		@function line invokes the line tool

		@note : This tool doesn't work yet
	*/
	void line();
	/*
		@function rectangle invokes the rectangle tool

		@note : This tool doesn't exist yet
	*/
	void rectangle();
	/*
		@function polygon invokes the polygon tool

		@note : This tool doesn't exist yet
	*/
	void polygon();
	/*
		@function circle invokes the circle tool

		@note : This tool doesn't exist yet
	*/
	void circle();
	/*
		@function threePointsArc invokes the threePointsArc tool

		@note : This tool doesn't exist yet
	*/
	void threePointsArc();

	/*
		@function dimension invokes the dimension tool

		@note : This tool doesn't exist yet
	*/
	void dimension();
	/*
		@function verticality invokes the verticality tool

		@note : This tool doesn't exist yet
	*/
	void verticality();
	/*
		@function horizontality invokes the horizontality tool

		@note : This tool doesn't exist yet
	*/
	void horizontality();
	/*
		@function perpendicularity invokes the perpendicularity tool

		@note : This tool doesn't exist yet
	*/
	void perpendicularity();
	/*
		@function parallelism invokes the parallelism tool

		@note : This tool doesn't exist yet
	*/
	void parallelism();
	/*
		@function coincidence invokes the coincidence tool

		@note : This tool doesn't exist yet
	*/
	void coincidence();
	/*
		@function equality invokes the equality tool

		@note : This tool doesn't exist yet
	*/
	void equality();

	/*
		@function to_svg saves the sketch as an svg file
	*/
	void to_svg();

	/*
		@function finish exits the sketch workspace
	*/
	void finish();
};

#endif