
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

#ifndef MISCUI_HPP_
#define MISCUI_HPP_

#include <string>

#include <gtkmm.h>

/*
	@class TabButton describes a tab with a label and a close button for the Document indexer
	@parent : Gtk::Box
*/
class TabButton : public Gtk::Box {
private:
	Gtk::Label mLabel; // The label set by the Document/the window
	Gtk::Button mCloseButton; // The close button, it is a 'x' character
public:
	/*
		@function TabButton creates a tab object

		@param label_ : The label on the tab
	*/
	TabButton(std::string const& label_);

	/*
		@function set_label sets the displayed label on the tab

		@param label_ : The label to be set
	*/
	void set_label(std::string const& label_) { mLabel.set_text(label_); }
	/*
		@function label 

		@return : The tab's text label
	*/
	std::string label() { return mLabel.get_text(); }
};

class StatusBar : public Gtk::Box {
public:
	enum status { WORKSPACE, TOOL };
private:
	Gtk::Label* workspaceName, * toolName;
public:
	StatusBar(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder);

	void update(status which, std::string const& content);
};

#endif