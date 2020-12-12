
#ifndef PARTDESIGN_HPP_
#define PARTDESIGN_HPP_

#include "workspace.hpp"

#include <string>

class partDesign : public workspace {
private:
	std::map<std::string, std::pair<Gtk::Button*, Gtk::Image*>> mButtons; // Buttons with names and icons
public:
	/*
		@function partDesign creates an empty workspace

		@param parent : The parent window of the workspace

		@note : Not recommended since it won't have buttons
	*/
	partDesign(bloop* parent);
	/*
		@function partDesign creates an empty workspace

		@param builder : 	The gtk builder that has all the info from an xml file
		@param parent : 	The parent window of the workspace	
	*/
	partDesign(Glib::RefPtr<Gtk::Builder> const& builder, bloop* parent);

	/*
		@function startSketch invokes the startSketch tool
	*/
	void startSketch();
	/*
		@function extrusion invokes the extrusion tool

		@note : This tool doesn't exist yet
	*/
	void extrusion();
};

#endif