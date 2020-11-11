
#ifndef NAVIGATIONBAR_HPP_
#define NAVIGATIONBAR_HPP_

#include <workspaces/workspace.hpp>

#include <gtkmm.h>

class navigationBar : public Gtk::Box {
private:
	Gtk::Button* mOrbitButton, *mPanButton, *mZoomButton, *mSelectorButton;
	Gtk::Image* mOrbitIcon, *mPanIcon, *mZoomIcon, *mSelectorIcon;

	std::shared_ptr<workspace> mCurrentWorkspace;
public:
	navigationBar(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder);

	void set_workspace(std::shared_ptr<workspace> aWorkspace);

	void notify_pan();
	void notify_orbit();
	void notify_zoom();
	void notify_selector();
};

#endif