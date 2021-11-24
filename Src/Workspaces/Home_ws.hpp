
// BloopCAD
// Copyright (C) 2020  BloopCorp

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

#ifndef HOME_HPP_
#define HOME_HPP_

#include "Workspace_abstr.hpp"

#include <map>

/*
	@class Home_ws describes the Home_ws workspace, it will eventually be the place to 
	create new files, open files and have previews, but now it is an Image and buttons that do nothing
	@parent : workspace
*/
class Home_ws : public Workspace_abstr {
private:
	std::map<std::string, std::pair<Gtk::Button*, Gtk::Image*>> mButtons; // Buttons with Images and names
public:
	/*
		@function Home_ws creates the Home_ws workspace with a xml file

		@param builder : 	The gtk builder that creates widget from an xml file
		@param parent : 	The window which it is part of
	*/
	Home_ws(Glib::RefPtr<Gtk::Builder> const& builder, Bloop* parent);
	~Home_ws();
	/*
		@function new_file will eventually create a file
	*/
	void new_file();
	/*
		@function open_file will eventually open an openfile dialog of some sort
	*/
	void open_file();

	virtual std::string name() { return "Home_ws"; }
};

#endif