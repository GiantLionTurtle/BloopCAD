
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

#include "NavigationBar.hpp"

#include <Utils/Expunge.hpp>
#include <Tools/Tools_forward.hpp>

NavigationBar::NavigationBar(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder)
	: Gtk::Box(cobject)
	, mCurrentWorkspace(nullptr)
{
	// Ask the builder to create all buttons
	builder->get_widget("orbit_btn", mOrbitButton);
	builder->get_widget("pan_btn", mPanButton);
	builder->get_widget("zoom_btn", mZoomButton);
	builder->get_widget("selector_btn", mSelectorButton);
	try { // Fetch all the buttons' icons
		mOrbitIcon 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Navigation/Orbit.png", 50, 50));
		mPanIcon 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Navigation/Pan.png", 50, 50));
		mZoomIcon 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Navigation/Zoom.png", 50, 50));
		mSelectorIcon 	= new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Navigation/Selector.png", 50, 50));
	} catch(const Glib::FileError& ex) {
		LOG_WARNING("Glib::FileError: " + ex.what());
	} catch(const Gdk::PixbufError& ex) {
		LOG_WARNING("Glib::PixbufError: " + ex.what());
	}
	// Assign all the buttons' icons
	mOrbitButton->set_image(*mOrbitIcon);
	mPanButton->set_image(*mPanIcon);
	mZoomButton->set_image(*mZoomIcon);
	mSelectorButton->set_image(*mSelectorIcon);

	// Bind all the buttons' callbacks to set the tools in the Workspaces
	mOrbitButton->signal_clicked().connect(sigc::mem_fun(*this, &NavigationBar::notify_orbit));
	mPanButton->signal_clicked().connect(sigc::mem_fun(*this, &NavigationBar::notify_pan));
	mZoomButton->signal_clicked().connect(sigc::mem_fun(*this, &NavigationBar::notify_zoom));
	mSelectorButton->signal_clicked().connect(sigc::mem_fun(*this, &NavigationBar::notify_selector));

	show_all(); // Ensures that nothing is hidden within gtk
}
NavigationBar::~NavigationBar()
{
	expunge(mOrbitIcon);
	expunge(mOrbitButton);
	
	expunge(mPanIcon);
	expunge(mPanButton);

	expunge(mZoomIcon);
	expunge(mZoomButton);

	expunge(mSelectorIcon);
	expunge(mSelectorButton);

	mCurrentWorkspace = nullptr;
}

void NavigationBar::set_workspace(Workspace_abstr* aWorkspace)
{
	mCurrentWorkspace = aWorkspace;
}

void NavigationBar::notify_pan()
{
	if(mCurrentWorkspace) {
		mCurrentWorkspace->set_tool(TOOLIDS::TOOLID_PAN);
	}
}
void NavigationBar::notify_orbit()
{
	if(mCurrentWorkspace) {
		mCurrentWorkspace->set_tool(TOOLIDS::TOOLID_ORBIT);
	}
}
void NavigationBar::notify_zoom()
{
	if(mCurrentWorkspace) {
		mCurrentWorkspace->set_tool(TOOLIDS::TOOLID_ZOOM);
	}
}
void NavigationBar::notify_selector()
{
	// if(mCurrentWorkspace) {
	// 	mCurrentWorkspace->set_tool("simpleSelector");
	// }
}