
#ifndef BLOOP_HPP_
#define BLOOP_HPP_

#include <Workspaces/Document.hpp>
#include <Tools/Tool.hpp>
#include <Workspaces/Workspace_abstr.hpp>
#include <Workspaces/NavigationBar.hpp>
#include <Workspaces/Part_ws.hpp>
#include <Workspaces/Sketch_ws.hpp>
#include <Workspaces/Home_ws.hpp>
#include <Workspaces/MiscUI.hpp>

#include <gtkmm.h>
#include <string>
#include <tuple>
#include <memory>

/*
	@class Bloop describes a window containing the whole cad software
	@parent : Gtk::Window

	@note : Kind of a dumpster fire at this point
*/
class Bloop : public Gtk::Window {
public:
	enum workspace_types { SKETCH, PART, HOME };
private:
	std::vector<std::tuple<TabButton, Gtk::Overlay, Document*>> mDocuments; // All the Document with their tab and overlay over which they render
	Document* mCurrentDocument; // The current Document object

	// std::map<std::string, Workspace_abstr*> mWorkspaces; // All the existing Workspaces indexed by name
	Sketch_ws* mSketchWorkspace;
	Part_ws* mPartDesignWorkspace;
	Home_ws* mHomeWorkspace;
	Workspace_abstr* mCurrentWorkspace; // The currently used workspace

	Glib::RefPtr<Gdk::Cursor> mCursor; // The current cursor
	Gtk::Image* mHomePage, *mIcon; // Program icon and Home_ws page Image (the latter will not be a thing)
	Gtk::Notebook* mDocumentIndexer; // The widget containing the Document rendering
	Gtk::Stack* mUI_upperBar, *mSideBar; // A stack of all the Workspaces upper bars and all the Documents' side bars
	Gtk::Overlay* mHome; // The Home_ws screen overlay (I guess that it could go with the Document's)
	NavigationBar* mNavigationBar; // The side navigation bar
	StatusBar* mStatusBar;

	glm::vec2 scroll_deltas; // kinda dirty but it's a workaround for a bug in gtk3
public:
	/*
		@function Bloop creates an empty Bloop window

		@note : Not recommended as it will just be an empty window
	*/
	Bloop();
	/*
		@function Bloop creates a Bloop window from an xml file

		@param cobject : The base c object from gtk
		@param builder : The gtk builder that has loaded an xml file for design
	*/
	Bloop(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder);
	~Bloop();
	/*
		@function currentDocument 

		@return : The currently used Document
	*/
	Document* currentDocument() { return mCurrentDocument; }
	void add_document(Document* doc);
	
	/*
		@function currentWorkspace

		@return the currently used workspace
	*/
	Workspace_abstr* currentWorkspace() { return mCurrentWorkspace; }
	Sketch_ws* sketchWorkspace() { return mSketchWorkspace; }
	Part_ws* partWorkspace() { return mPartDesignWorkspace; }
	Home_ws* Home_wsWorkspace() { return mHomeWorkspace; }

	/*
		@function set_workspace sets the currently used workspace with a named workspace

		@param name : 	The name of the workspace to use
		@param state : 	The state of the loaded workspace

		@return : The set workspace or nullptr
	*/
	Workspace_abstr* set_workspace(int name, WorkspaceState* state);
	Workspace_abstr* set_workspace(Workspace_abstr* wrkspc, WorkspaceState* state);
	void notify_set_tool(std::string const& name);

	void set_sideBar(Gtk::Widget* to_show);
	void add_sideBar(Gtk::Widget* sideBar);
	
	/*
		@function manage_mouse_scroll_internal keeps track of the scroll within the window
		because the scroll event captured by the Document doesn't have a delta y somewhy

		@param event : The scroll event handed by gtk
	*/
	bool manage_mouse_scroll_internal(GdkEventScroll* event);
	glm::vec2 scrollDeltas() const { return scroll_deltas; }

	/*
		@function manage_tab_switch handles the tab switching, sets up the 
		new current Document and cleans up the previous

		@param widget : The new current Document widget
		@param tab_ind : The tab index of the new current Document
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
		@function get_document_at_tabInd gets the Document at the specified index

		@param ind : The index of the Document of interest

		@return : The Document at the particular index or nullptr if it doesn't exist
	*/
	Document* get_document_at_tabInd(unsigned int ind);
};

#endif