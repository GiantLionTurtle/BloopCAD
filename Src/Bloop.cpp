
#include "Bloop.hpp"
#include <Utils/Debug_util.hpp>

Bloop::Bloop()
{
	set_border_width(6);

	maximize();
	show_all();
}

Bloop::Bloop(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder)
	: Gtk::Window(cobject)
	, mDocumentIndexer(nullptr)
	, mHomePage(nullptr)
	, mUI_upperBar(nullptr)
	, mHome(nullptr)
	, mIcon(nullptr)
{
	maximize(); // Go maximized, it deserves it

	Glib::RefPtr<Gtk::CssProvider> css = Gtk::CssProvider::create();
	if(!css->load_from_path("Resources/GUI/Style.css"))
		LOG_WARNING("Could not load styling info.");
	get_style_context()->add_provider_for_screen(Gdk::Screen::get_default(), css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	get_style_context()->add_class("Bloop");

	// Create all possible Workspaces
	mHomeWorkspace 			= new Home_ws(builder, this);
	mSketchWorkspace 		= new Sketch_ws(builder, this);
	mPartDesignWorkspace 	= new Part_ws(builder, this);

	// Create the window's widget
	builder->get_widget("DocumentIndexer", mDocumentIndexer);
	builder->get_widget("Home_wsPage", mHome);
	builder->get_widget("upperBar_stack", mUI_upperBar);
	builder->get_widget_derived("NavigationBar", mNavigationBar);
	builder->get_widget_derived("StatusBar", mStatusBar);
	builder->get_widget("sideBar", mSideBar);

	if(mHome && mDocumentIndexer) {
		try { // Fetch the icon and the Home_ws Image
			mHomePage = new Gtk::Image(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Normal/Chat.png"));
			mHome->add_overlay(*mHomePage);
			set_icon(Gdk::Pixbuf::create_from_file("Resources/Textures/Images/Icons/Icon.png", 400, 400));
		} catch(const Glib::FileError& ex) {
			LOG_WARNING("Glib file error: " + ex.what());
		} catch(const Gdk::PixbufError& ex) {
			LOG_WARNING("Gtk pixbuf error: " + ex.what());
		}
	} else {
		LOG_ERROR("Could not build Home_ws page.");
	}
	
	if(!mDocumentIndexer) {
		LOG_ERROR("Could not build Document indexer.");
	}

	if(!mSideBar) {
		LOG_ERROR("Could not build side bar.");
	}

	if(!mNavigationBar) {
		LOG_ERROR("Could not build navigation bar.");
	}
	
	if(!mStatusBar) {
		LOG_ERROR("Could not build status bar.");
	}
	
	// Add all possible upper bars in a stack
	if(mUI_upperBar) {
		mUI_upperBar->add(*mHomeWorkspace->upperBar());
		mUI_upperBar->add(*mSketchWorkspace->upperBar());
		mUI_upperBar->add(*mPartDesignWorkspace->upperBar());
	} else {
		LOG_ERROR("Cold not build upper bar.");
	}

	// Start at Home_ws workspace
	set_workspace(workspace_types::HOME, nullptr);

	connect_signals();
	show_all();	// Make sure nothing is hidden for some reason
}
Bloop::~Bloop()
{
	expunge(mSketchWorkspace);
	expunge(mPartDesignWorkspace);
	expunge(mHomeWorkspace);
	mCurrentWorkspace = nullptr;

	expunge(mHomePage);
	expunge(mIcon);
	
	expunge(mDocumentIndexer);
	expunge(mUI_upperBar);
	expunge(mSideBar); 
	expunge(mHome);
	
	expunge(mNavigationBar);
	expunge(mStatusBar);

	for(int i = 0; i < mDocuments.size(); ++i) {
		expunge(std::get<2>(mDocuments[i]));
	}
}

void Bloop::add_document(Document* doc)
{
	if(!doc) {
		LOG_WARNING("Trying to add null doc.");
		return;
	}
	mDocuments.push_back(std::make_tuple(TabButton(doc->name()), Gtk::Overlay(), doc));
	std::get<2>(mDocuments.back())->set_parent(this);

	mDocumentIndexer->append_page(std::get<1>(mDocuments.back()), std::get<0>(mDocuments.back()));
	std::get<1>(mDocuments.back()).add(*std::get<2>(mDocuments.back())); // Add the Document (for Gtk it is a box with a GLArea) to the new overlay

	std::get<1>(mDocuments.back()).add_overlay(*mNavigationBar);
	std::get<1>(mDocuments.back()).add_overlay(*mStatusBar);
	show_all();
}

Workspace_abstr* Bloop::set_workspace(int name, WorkspaceState* state)
{
	switch(name) {
	case workspace_types::HOME:
		mCurrentWorkspace = mHomeWorkspace;
		break;
	case workspace_types::SKETCH:
		mCurrentWorkspace = mSketchWorkspace;
		break;
	case workspace_types::PART:
		mCurrentWorkspace = mPartDesignWorkspace;
		break;
	default:
		LOG_WARNING("Trying to set workspace to \"" + std::to_string(name) + "\". No such workspace exist.");
		return nullptr;
	}
	mUI_upperBar->set_visible_child(*mCurrentWorkspace->upperBar()); // Update the upper bar
	mCurrentWorkspace->set_state(state); // Whoopsie, this surely should be fetching a new state from the Document. TODO: Fix this please
	mNavigationBar->set_workspace(mCurrentWorkspace);
	mStatusBar->update(StatusBar::WORKSPACE, mCurrentWorkspace->name());

	return mCurrentWorkspace;
}

void Bloop::notify_set_tool(std::string const& name)
{
	mStatusBar->update(StatusBar::status::TOOL, name);
}

void Bloop::set_sideBar(Gtk::Widget* to_show)
{
	mSideBar->set_visible_child(*to_show);
}
void Bloop::add_sideBar(Gtk::Widget* sideBar) 
{
	mSideBar->add(*sideBar, "part");
	show_all();
}

bool Bloop::manage_mouse_scroll_internal(GdkEventScroll* event)
{
	scroll_deltas = glm::vec2(event->delta_x, event->delta_y); // Record the scroll event
	return true;
}

void Bloop::manage_tab_switch(Gtk::Widget* widget, unsigned int tab_ind)
{
	Document* doc = get_document_at_tabInd(tab_ind);

	if(doc) { // If it's a doc do all the doc switching thingies (currently there can only be one Document, so there is surely problems here)
		mCurrentDocument = doc;
		mCurrentDocument->set_workspace();
	} else {
		mCurrentDocument = nullptr;
		set_workspace(workspace_types::HOME, nullptr);
	}
}

void Bloop::set_cursor(Glib::RefPtr<Gdk::Cursor> cursor_)
{
	mCursor = cursor_;
	get_window()->set_cursor(cursor_);
}

Document* Bloop::get_document_at_tabInd(unsigned int ind)
{
	if(ind == 0 || ind > mDocuments.size())
		return nullptr;
	return std::get<2>(mDocuments.at(ind-1));
}

void Bloop::connect_signals()
{
	// I don't know if setting all the masks is really usefull since it is set in Document and other parts as well	
	add_events( 		  Gdk::POINTER_MOTION_MASK
						| Gdk::BUTTON_PRESS_MASK
						| Gdk::BUTTON_RELEASE_MASK
						| Gdk::KEY_PRESS_MASK
						| Gdk::KEY_RELEASE_MASK
						| Gdk::STRUCTURE_MASK
						| Gdk::SCROLL_MASK);
	// signal_key_press_event().connect(sigc::mem_fun(*this, &Bloop::manage_key_press));
	signal_scroll_event().connect(sigc::mem_fun(*this, &Bloop::manage_mouse_scroll_internal));
	mDocumentIndexer->signal_switch_page().connect(sigc::mem_fun(*this, &Bloop::manage_tab_switch)); 
}