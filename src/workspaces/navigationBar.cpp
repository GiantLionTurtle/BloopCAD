
#include "navigationBar.hpp"

navigationBar::navigationBar(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder) :
	Gtk::Box(cobject),
	mCurrentWorkspace(nullptr)
{
	builder->get_widget("orbit_btn", mOrbitButton);
	builder->get_widget("pan_btn", mPanButton);
	builder->get_widget("zoom_btn", mZoomButton);
	builder->get_widget("selector_btn", mSelectorButton);
	try {
		mOrbitIcon 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/navigation/orbit.png", 50, 50));
		mPanIcon 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/navigation/pan.png", 50, 50));
		mZoomIcon 		= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/navigation/zoom.png", 50, 50));
		mSelectorIcon 	= new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/navigation/selector.png", 50, 50));
	} catch(const Glib::FileError& ex) {
		LOG_WARNING("Glib::FileError: " + ex.what());
	} catch(const Gdk::PixbufError& ex) {
		LOG_WARNING("Glib::PixbufError: " + ex.what());
	}
	mOrbitButton->set_image(*mOrbitIcon);
	mOrbitButton->signal_clicked().connect(sigc::mem_fun(*this, &navigationBar::notify_orbit));

	mPanButton->set_image(*mPanIcon);
	mPanButton->signal_clicked().connect(sigc::mem_fun(*this, &navigationBar::notify_pan));

	mZoomButton->set_image(*mZoomIcon);
	mZoomButton->signal_clicked().connect(sigc::mem_fun(*this, &navigationBar::notify_zoom));

	mSelectorButton->set_image(*mSelectorIcon);
	mSelectorButton->signal_clicked().connect(sigc::mem_fun(*this, &navigationBar::notify_selector));

	show_all();
}

void navigationBar::set_workspace(std::shared_ptr<workspace> aWorkspace)
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