
#ifndef PARTDESIGN_HPP_
#define PARTDESIGN_HPP_

#include "workspace.hpp"
#include <Drawables/Part.hpp>

#include <string>

class partDesign : public workspace {
private:
	std::shared_ptr<Pan3d_tool> mPan3d_tool;
	std::shared_ptr<PartDefault_tool> mPartDefault_tool;
	std::shared_ptr<StartSketch_tool> mStartSketch_tool;

	std::map<std::string, std::pair<Gtk::Button*, Gtk::Image*>> mButtons; // Buttons with names and icons

	Part* mTarget;
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
	~partDesign();

	void set_target(Part* prt) { mTarget = prt; }
	Part* target() { return mTarget; }

	virtual bool set_tool(int name);

	bool manage_key_press(GdkEventKey* event);

	std::shared_ptr<Pan3d_tool> pan() { return mPan3d_tool; }
	std::shared_ptr<PartDefault_tool> partDesignDefault() { return mPartDefault_tool; }
	std::shared_ptr<StartSketch_tool> startSketch() { return mStartSketch_tool; }

	/*
		@function startSketch invokes the startSketch tool
	*/
	void begin_startSketch();
	/*
		@function extrusion invokes the extrusion tool

		@note : This tool doesn't exist yet
	*/
	void begin_extrusion();

	virtual std::string name() { return "part design"; }
};

#endif