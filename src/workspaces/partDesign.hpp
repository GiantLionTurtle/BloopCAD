
#ifndef PARTDESIGN_HPP_
#define PARTDESIGN_HPP_

#include "workspace.hpp"
#include <entities/part.hpp>

#include <string>

class partDesign : public workspace {
private:
	std::map<std::string, std::pair<Gtk::Button*, Gtk::Image*>> mButtons; // Buttons with names and icons

	part_ptr mTarget;
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

	void set_target(part_ptr prt) { mTarget = prt; }
	part_ptr target() { return mTarget; }

	/*
		@function startSketch invokes the startSketch tool
	*/
	void startSketch();
	/*
		@function extrusion invokes the extrusion tool

		@note : This tool doesn't exist yet
	*/
	void extrusion();

	virtual std::string name() { return "part design"; }
};

#endif