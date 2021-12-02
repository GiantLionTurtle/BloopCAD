
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

#ifndef PARTDESIGN_HPP_
#define PARTDESIGN_HPP_

#include "Workspace_abstr.hpp"
#include <Drawables/Containers/Part.hpp>

#include <string>

class Part_ws : public Workspace_abstr {
private:
	Pan3d_tool* mPan3d_tool;
	PartDefault_tool* mPartDefault_tool;
	StartSketch_tool* mStartSketch_tool;

	std::map<std::string, std::pair<Gtk::Button*, Gtk::Image*>> mButtons; // Buttons with names and icons

	Part* mTarget;
public:
	/*
		@function Part_ws creates an empty workspace

		@param builder : 	The gtk builder that has all the info from an xml file
		@param parent : 	The parent window of the workspace	
	*/
	Part_ws(Glib::RefPtr<Gtk::Builder> const& builder, Bloop* parent);
	~Part_ws();

	void set_target(Part* prt) { mTarget = prt; }
	Part* target() { return mTarget; }

	virtual bool set_tool(int name);

	bool manage_key_press(GdkEventKey* event);

	Pan3d_tool* pan() { return mPan3d_tool; }
	PartDefault_tool* default_tool() { return mPartDefault_tool; }
	StartSketch_tool* startSketch() { return mStartSketch_tool; }

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