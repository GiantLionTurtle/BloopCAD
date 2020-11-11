
#include "bloop.hpp"
#include "errorLogger.hpp"
#include "workspaces/partDesign.hpp"
#include "workspaces/sketchDesign.hpp"
#include "workspaces/home.hpp"

bloop::bloop()
{
	set_border_width(6);

	mCurrentDocument = std::shared_ptr<document>(new document(this));
	mContainer.pack_end(*mCurrentDocument, Gtk::PACK_EXPAND_WIDGET);
	add(mContainer);

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
	maximize();

	mWorkspaces["home"]			= std::shared_ptr<workspace>(new home(builder, this));
	mWorkspaces["sketchDesign"] = std::shared_ptr<workspace>(new sketchDesign(builder, this));
	mWorkspaces["partDesign"] 	= std::shared_ptr<workspace>(new partDesign(builder, this));

	builder->get_widget("documentIndexer", mDocumentIndexer);
	builder->get_widget("homePage", mHome);
	builder->get_widget("upperBar_stack", mUI_upperBar);
	builder->get_widget_derived("navigationBar", mNavigationBar);
	builder->get_widget("sideBar", mSideBar);

	if(mHome && mDocumentIndexer) {
		try {
			mHomePage = new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/normal/sunset.png"));
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
	
	if(mDocumentIndexer) {
		mDocumentIndexer->signal_switch_page().connect(sigc::mem_fun(*this, &bloop::manage_tab_switch));

		// Warning: this is for testing purposes.
		mDocuments.push_back(std::make_tuple(tabButton("Document"), Gtk::Overlay(), std::shared_ptr<document>(new document(this))));
		mDocumentIndexer->append_page(std::get<1>(mDocuments.back()), std::get<0>(mDocuments.back()));
		//mDocumentIndexer->set_tab_reorderable(std::get<1>(mDocuments.back()));

		std::get<1>(mDocuments.back()).add(*std::get<2>(mDocuments.back())); // Add the document (for Gtk it is a box with a GLArea) to the new overlay
	} else {
		LOG_ERROR("Could not build document indexer.");
	}

	if(mNavigationBar) {
		std::get<1>(mDocuments.back()).add_overlay(*mNavigationBar);
	} else {
		LOG_ERROR("Could not build navigation bar.");
	}
	
	if(mUI_upperBar) {
		mUI_upperBar->add(*mWorkspaces.at("home")->upperBar());
		mUI_upperBar->add(*mWorkspaces.at("sketchDesign")->upperBar());
		mUI_upperBar->add(*mWorkspaces.at("partDesign")->upperBar());
	} else {
		LOG_ERROR("Cold not build upper bar.");
	}

	set_workspace("home");

	connect_signals();
	show_all();	
}

void bloop::set_workspace(std::string const& name)
{
	if(mWorkspaces.find(name) != mWorkspaces.end()) {
		mUI_upperBar->set_visible_child(*mWorkspaces.at(name)->upperBar());
		mCurrentWorkspace = mWorkspaces.at(name);
		if(mNavigationBar)
			mNavigationBar->set_workspace(mCurrentWorkspace);
	} else {
		LOG_WARNING("Trying to set workspace to \"" + name + "\". No such workspace exist.");
	}
}

void bloop::set_workspaceState(std::shared_ptr<workspaceState> state)
{
//	std::cout<<__FILE__<<",  "<<__LINE__<<" : Disabled\n\n";
	if(mCurrentWorkspace) {
		mCurrentWorkspace->set_state(state);
 	} else {
		LOG_WARNING("There are no current workspace in bloop");
	}
}

bool bloop::manage_key_press(GdkEventKey* event)
{
	if(mCurrentWorkspace) {
		return mCurrentWorkspace->manage_key_press(event);
	}
	return true;
}
bool bloop::manage_mouse_move(GdkEventMotion* event)
{
	if(mCurrentWorkspace) {
		return mCurrentWorkspace->manage_mouse_move(event);
	}
	return true;
}

bool bloop::manage_mouse_scroll_internal(GdkEventScroll* event)
{
	scroll_deltas = glm::vec2(event->delta_x, event->delta_y);
	return true;
}

bool bloop::manage_mouse_scroll(GdkEventScroll* event)
{
	if(mCurrentWorkspace) {
		event->delta_x = scroll_deltas.x;
		event->delta_y = scroll_deltas.y;
		return mCurrentWorkspace->manage_mouse_scroll(event);
	}
	return true;
}
bool bloop::manage_button_press(GdkEventButton* event)
{
	if(mCurrentWorkspace) {
		return mCurrentWorkspace->manage_button_press(event);
	}
	return true;
}
bool bloop::manage_button_release(GdkEventButton* event)
{
	if(mCurrentWorkspace) {
		return mCurrentWorkspace->manage_button_release(event);
	}
	return true;
}

void bloop::manage_tab_switch(Gtk::Widget* widget, unsigned int tab_ind)
{
	std::shared_ptr<document> doc = get_document_at_tabInd(tab_ind);

	if(doc) {
		mCurrentDocument = doc;
		mCurrentDocument->set_workspace();
	} else {
		mCurrentDocument = nullptr;
		set_workspace("home");
	}
}

std::shared_ptr<document> bloop::get_document_at_tabInd(unsigned int ind)
{
	if(ind == 0 || ind > mDocuments.size())
		return nullptr;
	return std::get<2>(mDocuments.at(ind-1));
}

void bloop::connect_signals()
{
	
	add_events( 		  Gdk::POINTER_MOTION_MASK
						| Gdk::BUTTON_PRESS_MASK
						| Gdk::BUTTON_RELEASE_MASK
						| Gdk::KEY_PRESS_MASK
						| Gdk::KEY_RELEASE_MASK
						| Gdk::STRUCTURE_MASK
						| Gdk::SCROLL_MASK);
	signal_key_press_event().connect(sigc::mem_fun(*this, &bloop::manage_key_press));
	//signal_motion_notify_event().connect(sigc::mem_fun(*this, &bloop::manage_mouse_move));
	signal_scroll_event().connect(sigc::mem_fun(*this, &bloop::manage_mouse_scroll_internal)); // workaroound
	// signal_button_release_event().connect(sigc::mem_fun(*this, &bloop::manage_button_release));	
}