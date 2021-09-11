
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