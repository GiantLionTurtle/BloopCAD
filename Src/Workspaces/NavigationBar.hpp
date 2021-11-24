
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

#ifndef NAVIGATIONBAR_HPP_
#define NAVIGATIONBAR_HPP_

#include <Workspaces/Workspace_abstr.hpp>

#include <gtkmm.h>

/*
	@class NavigationBar describes an overlaid box containing navigation tools
	@parent : Gtk::Box
*/
class NavigationBar : public Gtk::Box {
private:
	Gtk::Button* mOrbitButton, *mPanButton, *mZoomButton, *mSelectorButton; // All the navigation tools
	Gtk::Image* mOrbitIcon, *mPanIcon, *mZoomIcon, *mSelectorIcon; // All the navigation tools icons

	Workspace_abstr* mCurrentWorkspace;
public:
	/*
		@function navigation bar creates the object as a gtk box

		@param cobject :	The Gtk::Box base c object
		@param builder : 	The gtk widget builder that creates widgets from xml files
	*/
	NavigationBar(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder);
	~NavigationBar();
	/*
		@function set_workspace tells the navigation bar in which worspace it is currently being used

		@param aWorkspace : The workspace using the navigation bar
	*/
	void set_workspace(Workspace_abstr* aWorkspace);

	/*
		@function notify_pan sends a message to the workspace to use the pan tool
	*/
	void notify_pan();
	/*
		@function notify_orbit sends a message to the workspace to use the orbit tool
	*/
	void notify_orbit();
	/*
		@function notify_zoom sends a message to the workspace to use the zoom tool
	*/
	void notify_zoom();
	/*
		@function notify_selector sends a message to the workspace to use the selector tool
	*/
	void notify_selector();
};

#endif