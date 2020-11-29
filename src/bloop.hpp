
#ifndef BLOOP_HPP_
#define BLOOP_HPP_

#include "forward_bloop.hpp"
#include "document.hpp"
#include <tools/tool.hpp>
#include <workspaces/workspace.hpp>
#include <workspaces/navigationBar.hpp>

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

class bloop : public Gtk::Window {
private:
	std::vector<std::tuple<tabButton, Gtk::Overlay, std::shared_ptr<document>>> mDocuments;
	std::shared_ptr<document> mCurrentDocument;

	std::map<std::string, std::shared_ptr<workspace>> mWorkspaces;
	std::shared_ptr<workspace> mCurrentWorkspace;

	Gtk::Box mContainer;

	std::shared_ptr<compositeCursor> mCursor;
	Gtk::Image* mCursorImg;
	Gtk::Image* mHomePage, *mIcon;
	Gtk::Notebook* mDocumentIndexer;
	Gtk::Stack* mUI_upperBar, *mSideBar;
	Gtk::Overlay* mHome;
	navigationBar* mNavigationBar;

	glm::vec2 scroll_deltas; // kinda dirty but it's a workaround for a bug in gtk3
public:
	bloop();
	bloop(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder);

	std::shared_ptr<document> currentDocument() { return mCurrentDocument; }
	std::shared_ptr<workspace> currentWorkspace() { return mCurrentWorkspace; }

	void set_workspace(std::string const& name, std::shared_ptr<workspaceState> state);

	bool manage_key_press(GdkEventKey* event);
	bool manage_key_release(GdkEventKey* event);
	bool manage_mouse_move(GdkEventMotion* event);
	bool manage_mouse_scroll(GdkEventScroll* event);
	bool manage_mouse_scroll_internal(GdkEventScroll* event);
	bool manage_button_press(GdkEventButton* event);
	bool manage_button_release(GdkEventButton* event);

	void manage_tab_switch(Gtk::Widget* widget, unsigned int tab_ind);

	void connect_signals();

	void set_cursor(std::shared_ptr<compositeCursor> cursor_);

	std::shared_ptr<document> get_document_at_tabInd(unsigned int ind);
};

#endif