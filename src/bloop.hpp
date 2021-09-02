
#ifndef BLOOP_HPP_
#define BLOOP_HPP_

#include "forward_bloop.hpp"
#include "document.hpp"
#include <Tools/Tool.hpp>
#include <workspaces/workspace.hpp>
#include <workspaces/navigationBar.hpp>
#include "workspaces/partDesign.hpp"
#include "workspaces/sketchDesign.hpp"
#include "workspaces/home.hpp"

#include <gtkmm.h>
#include <string>
#include <tuple>
#include <memory>

/*
	@class tabButton describes a tab with a label and a close button for the document indexer
	@parent : Gtk::Box
*/
class tabButton : public Gtk::Box {
private:
	Gtk::Label mLabel; // The label set by the document/the window
	Gtk::Button mCloseButton; // The close button, it is a 'x' character
public:
	/*
		@function tabButton creates a tab object

		@param label_ : The label on the tab
	*/
	tabButton(std::string const& label_);

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

class statusBar : public Gtk::Box {
public:
	enum status { WORKSPACE, TOOL };
private:
	Gtk::Label* workspaceName, * toolName;
public:
	statusBar(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder);

	void update(status which, std::string const& content);
};


/*
	@class bloop describes a window containing the whole cad software
	@parent : Gtk::Window

	@note : Kind of a dumpster fire at this point
*/
class bloop : public Gtk::Window {
public:
	enum workspace_types { SKETCH, PART, HOME };
private:
	std::vector<std::tuple<tabButton, Gtk::Overlay, document_ptr>> mDocuments; // All the document with their tab and overlay over which they render
	document_ptr mCurrentDocument; // The current document object

	// std::map<std::string, workspace_ptr> mWorkspaces; // All the existing workspaces indexed by name
	std::shared_ptr<sketchDesign> mSketchWorkspace;
	std::shared_ptr<partDesign> mPartDesignWorkspace;
	std::shared_ptr<home> mHomeWorkspace;
	workspace_ptr mCurrentWorkspace; // The currently used workspace

	Glib::RefPtr<Gdk::Cursor> mCursor; // The current cursor
	Gtk::Image* mHomePage, *mIcon; // Program icon and home page Image (the latter will not be a thing)
	Gtk::Notebook* mDocumentIndexer; // The widget containing the document rendering
	Gtk::Stack* mUI_upperBar, *mSideBar; // A stack of all the workspaces upper bars and all the documents' side bars
	Gtk::Overlay* mHome; // The home screen overlay (I guess that it could go with the document's)
	navigationBar* mNavigationBar; // The side navigation bar
	statusBar* mStatusBar;

	glm::vec2 scroll_deltas; // kinda dirty but it's a workaround for a bug in gtk3
public:
	/*
		@function bloop creates an empty bloop window

		@note : Not recommended as it will just be an empty window
	*/
	bloop();
	/*
		@function bloop creates a bloop window from an xml file

		@param cobject : The base c object from gtk
		@param builder : The gtk builder that has loaded an xml file for design
	*/
	bloop(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder);

	/*
		@function currentDocument 

		@return : The currently used document
	*/
	document_ptr currentDocument() { return mCurrentDocument; }
	void add_document(document_ptr doc);
	
	/*
		@function currentWorkspace

		@return the currently used workspace
	*/
	workspace_ptr currentWorkspace() { return mCurrentWorkspace; }
	std::shared_ptr<sketchDesign> sketchWorkspace() { return mSketchWorkspace; }
	std::shared_ptr<partDesign> partWorkspace() { return mPartDesignWorkspace; }
	std::shared_ptr<home> homeWorkspace() { return mHomeWorkspace; }

	/*
		@function set_workspace sets the currently used workspace with a named workspace

		@param name : 	The name of the workspace to use
		@param state : 	The state of the loaded workspace

		@return : The set workspace or nullptr
	*/
	workspace_ptr set_workspace(int name, workspaceState_ptr state);
	workspace_ptr set_workspace(workspace_ptr wrkspc, workspaceState_ptr state);
	void notify_set_tool(std::string const& name);

	void set_sideBar(Gtk::Widget* to_show);
	void add_sideBar(Gtk::Widget* sideBar);
	
	/*
		@function manage_mouse_scroll_internal keeps track of the scroll within the window
		because the scroll event captured by the document doesn't have a delta y somewhy

		@param event : The scroll event handed by gtk
	*/
	bool manage_mouse_scroll_internal(GdkEventScroll* event);
	glm::vec2 scrollDeltas() const { return scroll_deltas; }

	/*
		@function manage_tab_switch handles the tab switching, sets up the 
		new current document and cleans up the previous

		@param widget : The new current document widget
		@param tab_ind : The tab index of the new current document
	*/
	void manage_tab_switch(Gtk::Widget* widget, unsigned int tab_ind);

	/*
		@function connect_signals connects all the window's callbacks
	*/
	void connect_signals();

	/*
		@function set_cursor sets the current cursor from a cursor object

		@param cursor_ : The cursor object to become the window's cursor
	*/
	void set_cursor(Glib::RefPtr<Gdk::Cursor> cursor_);

	/*
		@function get_document_at_tabInd gets the document at the specified index

		@param ind : The index of the document of interest

		@return : The document at the particular index or nullptr if it doesn't exist
	*/
	document_ptr get_document_at_tabInd(unsigned int ind);
};

#endif