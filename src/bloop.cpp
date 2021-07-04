
#include "bloop.hpp"
#include <utils/DebugUtils.hpp>

statusBar::statusBar(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder):
	Gtk::Box(cobject)
{
	builder->get_widget("status_workspace", workspaceName);
	builder->get_widget("status_tool", toolName);
}

void statusBar::update(status which, std::string const& content)
{
	if(which == status::WORKSPACE) {
		workspaceName->set_label(content);
	} else if(which == status::TOOL) {
		toolName->set_label(content);
	} else {
		LOG_WARNING("Attempted to set unknown status: " + std::to_string(which) + ".");
	}
}


bloop::bloop()
{
	set_border_width(6);

	maximize();
	show_all();
}

bloop::bloop(BaseObjectType* cobject, Glib::RefPtr<Gtk::Builder> const& builder) :
	Gtk::Window(cobject),
	mDocumentIndexer(nullptr),
	mHomePage(nullptr),
	mUI_upperBar(nullptr),
	mHome(nullptr),
	mIcon(nullptr)
{
	maximize(); // Go maximized, it deserves it

	Glib::RefPtr<Gtk::CssProvider> css = Gtk::CssProvider::create();
	if(!css->load_from_path("resources/GUI/style.css"))
		LOG_WARNING("Could not load styling info.");
	get_style_context()->add_provider_for_screen(Gdk::Screen::get_default(), css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	get_style_context()->add_class("bloop");

	// Create all possible workspaces
	mHomeWorkspace 			= std::shared_ptr<home>(new home(builder, this));
	mSketchWorkspace 		= std::shared_ptr<sketchDesign>(new sketchDesign(builder, this));
	mPartDesignWorkspace 	= std::shared_ptr<partDesign>(new partDesign(builder, this));

	// Create the window's widget
	builder->get_widget("documentIndexer", mDocumentIndexer);
	builder->get_widget("homePage", mHome);
	builder->get_widget("upperBar_stack", mUI_upperBar);
	builder->get_widget_derived("navigationBar", mNavigationBar);
	builder->get_widget_derived("statusBar", mStatusBar);
	builder->get_widget("sideBar", mSideBar);

	if(mHome && mDocumentIndexer) {
		try { // Fetch the icon and the home Image
			mHomePage = new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/normal/chat.png"));
			mHome->add_overlay(*mHomePage);
			set_icon(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/icon.png", 400, 400));
		} catch(const Glib::FileError& ex) {
			LOG_WARNING("Glib file error: " + ex.what());
		} catch(const Gdk::PixbufError& ex) {
			LOG_WARNING("Gtk pixbuf error: " + ex.what());
		}
	} else {
		LOG_ERROR("Could not build home page.");
	}
	
	if(!mDocumentIndexer) {
		LOG_ERROR("Could not build document indexer.");
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

	// Start at home workspace
	set_workspace(workspace_types::HOME, nullptr);

	connect_signals();
	show_all();	// Make sure nothing is hidden for some reason
}

void bloop::add_document(document_ptr doc)
{
	if(!doc) {
		LOG_WARNING("Trying to add null doc.");
		return;
	}
	mDocuments.push_back(std::make_tuple(tabButton(doc->name()), Gtk::Overlay(), doc));
	std::get<2>(mDocuments.back())->set_parent(this);

	mDocumentIndexer->append_page(std::get<1>(mDocuments.back()), std::get<0>(mDocuments.back()));
	std::get<1>(mDocuments.back()).add(*std::get<2>(mDocuments.back())); // Add the document (for Gtk it is a box with a GLArea) to the new overlay

	std::get<1>(mDocuments.back()).add_overlay(*mNavigationBar);
	std::get<1>(mDocuments.back()).add_overlay(*mStatusBar);
	show_all();
}

workspace_ptr bloop::set_workspace(int name, std::shared_ptr<workspaceState> state)
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
	mCurrentWorkspace->set_state(state); // Whoopsie, this surely should be fetching a new state from the document. TODO: Fix this please
	mNavigationBar->set_workspace(mCurrentWorkspace);
	mStatusBar->update(statusBar::WORKSPACE, mCurrentWorkspace->name());

	return mCurrentWorkspace;
}

void bloop::notify_set_tool(std::string const& name)
{
	mStatusBar->update(statusBar::status::TOOL, name);
}

void bloop::set_sideBar(Gtk::Widget* to_show)
{
	mSideBar->set_visible_child(*to_show);
}
void bloop::add_sideBar(Gtk::Widget* sideBar) 
{
	mSideBar->add(*sideBar, "part");
	show_all();
}

bool bloop::manage_mouse_scroll_internal(GdkEventScroll* event)
{
	scroll_deltas = glm::vec2(event->delta_x, event->delta_y); // Record the scroll event
	return true;
}

void bloop::manage_tab_switch(Gtk::Widget* widget, unsigned int tab_ind)
{
	std::shared_ptr<document> doc = get_document_at_tabInd(tab_ind);

	if(doc) { // If it's a doc do all the doc switching thingies (currently there can only be one document, so there is surely problems here)
		mCurrentDocument = doc;
		mCurrentDocument->set_workspace();
	} else {
		mCurrentDocument = nullptr;
		set_workspace(workspace_types::HOME, nullptr);
	}
}

void bloop::set_cursor(std::shared_ptr<compositeCursor> cursor_)
{
	mCursor = cursor_;
	get_window()->set_cursor(cursor_->windowCursor);
}

std::shared_ptr<document> bloop::get_document_at_tabInd(unsigned int ind)
{
	if(ind == 0 || ind > mDocuments.size())
		return nullptr;
	return std::get<2>(mDocuments.at(ind-1));
}

void bloop::connect_signals()
{
	// I don't know if setting all the masks is really usefull since it is set in document and other parts as well	
	add_events( 		  Gdk::POINTER_MOTION_MASK
						| Gdk::BUTTON_PRESS_MASK
						| Gdk::BUTTON_RELEASE_MASK
						| Gdk::KEY_PRESS_MASK
						| Gdk::KEY_RELEASE_MASK
						| Gdk::STRUCTURE_MASK
						| Gdk::SCROLL_MASK);
	// signal_key_press_event().connect(sigc::mem_fun(*this, &bloop::manage_key_press));
	signal_scroll_event().connect(sigc::mem_fun(*this, &bloop::manage_mouse_scroll_internal));
	mDocumentIndexer->signal_switch_page().connect(sigc::mem_fun(*this, &bloop::manage_tab_switch)); 
}