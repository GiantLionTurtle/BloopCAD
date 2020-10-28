
#ifndef BLOOP_HPP_
#define BLOOP_HPP_

#include "forward_bloop.hpp"
#include "document.hpp"
#include "tools/tool.hpp"
#include "workspaces/workspace.hpp"

#include <gtkmm.h>
#include <string>
#include <tuple>
#include <memory>

class tabButton : public Gtk::Box {
private:
	Gtk::Label* mLabel;
	Gtk::Button* mCloseButton;
public:
	tabButton(std::string const& label) 
	{
		mLabel = new Gtk::Label(label);
		mCloseButton = new Gtk::Button("x");
		mCloseButton->set_relief(Gtk::ReliefStyle::RELIEF_NONE);

		pack_start(*mLabel);
		pack_start(*mCloseButton);

		show_all();
	}
	void set_label(std::string const& label) { mLabel->set_text(label); }
	std::string get_label() { return mLabel->get_text(); }
};

class navigationBar : public Gtk::Box {
private:
	Gtk::Button* mOrbitButton, *mPanButton, *mZoomButton, *mSelectorButton;
	Gtk::Image* mOrbitIcon, *mPanIcon, *mZoomIcon, *mSelectorIcon;

	std::shared_ptr<workspace> mCurrentWorkspace;
public:
	navigationBar(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder) :
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
			LOG_ERROR("Glib::FileError: " + ex.what());
		} catch(const Gdk::PixbufError& ex) {
			LOG_ERROR("Glib::PixbufError: " + ex.what());
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

	void set_workspace(std::shared_ptr<workspace> aWorkspace)
	{
		mCurrentWorkspace = aWorkspace;
	}

	void notify_pan()
	{
		// mCurrentWorkspace->set_tool("pan");
		std::cout<<"pan button not implemented yet...\n";
	}
	void notify_orbit()
	{
		if(mCurrentWorkspace) {
			mCurrentWorkspace->set_tool("orbit");
		}
	}
	void notify_zoom()
	{
		if(mCurrentWorkspace) {
			mCurrentWorkspace->set_tool("zoom");
		}
	}
	void notify_selector()
	{
		if(mCurrentWorkspace) {
			mCurrentWorkspace->set_tool("simpleSelector");
		}
	}

};


class bloop : public Gtk::Window {
private:
	std::vector<std::tuple<tabButton, Gtk::Overlay, std::shared_ptr<document>>> mDocuments;
	std::shared_ptr<document> mCurrentDocument;

	std::map<std::string, std::shared_ptr<workspace>> mWorkspaces;
	std::shared_ptr<workspace> mCurrentWorkspace;

	Gtk::Box mContainer;

	Gtk::Image* mHomePage, * mIcon;
	Gtk::Notebook* mDocumentIndexer;
	Gtk::Stack* mUI_upperBar;
	Gtk::Overlay* mHome;
	navigationBar* mNavigationBar;
public:
	bloop();
	bloop(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder);

	std::shared_ptr<document> currentDocument() { return mCurrentDocument; }
	void set_workspaceState(std::shared_ptr<workspaceState> state);

	void set_workspace(std::string const& name, std::shared_ptr<entity> target);

	bool manage_key_press(GdkEventKey* event);
	bool manage_mouse_move(GdkEventMotion* event);
	bool manage_mouse_scroll(GdkEventScroll* event);
	bool manage_button_release(GdkEventButton* event);

	void set_tool(GdkEventKey* event);
	void set_tool(std::string const& name);

	void connect_signals();
};

#endif