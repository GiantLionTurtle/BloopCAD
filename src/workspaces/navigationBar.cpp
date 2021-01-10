
#include "navigationBar.hpp"

navigationBar::navigationBar(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder) :
	Gtk::Box(cobject),
	mCurrentWorkspace(nullptr)
{
	// Ask the builder to create all buttons
	builder->get_widget("orbit_btn", mOrbitButton);
	builder->get_widget("pan_btn", mPanButton);
	builder->get_widget("zoom_btn", mZoomButton);
	builder->get_widget("selector_btn", mSelectorButton);
	try { // Fetch all the buttons' icons
		mOrbitIcon 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/navigation/orbit.png", 50, 50));
		mPanIcon 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/navigation/pan.png", 50, 50));
		mZoomIcon 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/navigation/zoom.png", 50, 50));
		mSelectorIcon 	= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/navigation/selector.png", 50, 50));
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

	// Bind all the buttons' callbacks to set the tools in the workspaces
	mOrbitButton->signal_clicked().connect(sigc::mem_fun(*this, &navigationBar::notify_orbit));
	mPanButton->signal_clicked().connect(sigc::mem_fun(*this, &navigationBar::notify_pan));
	mZoomButton->signal_clicked().connect(sigc::mem_fun(*this, &navigationBar::notify_zoom));
	mSelectorButton->signal_clicked().connect(sigc::mem_fun(*this, &navigationBar::notify_selector));

	show_all(); // Ensures that nothing is hidden within gtk
}

void navigationBar::set_workspace(workspace_ptr aWorkspace)
{
	mCurrentWorkspace = aWorkspace;
}

void navigationBar::notify_pan()
{
	if(mCurrentWorkspace) {
		mCurrentWorkspace->set_tool("pan");
	}
}
void navigationBar::notify_orbit()
{
	if(mCurrentWorkspace) {
		mCurrentWorkspace->set_tool("orbit");
	}
}
void navigationBar::notify_zoom()
{
	if(mCurrentWorkspace) {
		mCurrentWorkspace->set_tool("zoom");
	}
}
void navigationBar::notify_selector()
{
	if(mCurrentWorkspace) {
		mCurrentWorkspace->set_tool("simpleSelector");
	}
}