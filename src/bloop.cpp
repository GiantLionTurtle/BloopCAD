
#include "bloop.hpp"
#include "errorLogger.hpp"
#include "workspaces/partDesign.hpp"
#include "workspaces/sketchDesign.hpp"

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

	mWorkspaces["sketchDesign"] = std::shared_ptr<workspace>(new sketchDesign(builder, this));
	mWorkspaces["partDesign"] 	= std::shared_ptr<workspace>(new partDesign(builder, this));

	builder->get_widget("documentIndexer", mDocumentIndexer);
	builder->get_widget("homePage", mHome);
	builder->get_widget("upperBar_stack", mUI_upperBar);
	builder->get_widget_derived("navigationBar", mNavigationBar);

	if(mHome && mDocumentIndexer) {
		try {
			mHomePage = new Gtk::Image(Gdk::Pixbuf::create_from_file("resources/textures/images/normal/sunset.png"));
			mHomePage->set_tooltip_text("Home");
			mHome->add_overlay(*mHomePage);

			set_icon(Gdk::Pixbuf::create_from_file("resources/textures/images/icons/icon.png", 400, 400));
		} catch(const Glib::FileError& ex) {
			LOG_ERROR("Glib file error: " + ex.what());
		} catch(const Gdk::PixbufError& ex) {
			LOG_ERROR("Gtk pixbuf error: " + ex.what());
		}
	} else {
		LOG_ERROR("Could not build home page.");
	}
	
	if(mDocumentIndexer) {
		mDocuments.push_back(std::make_tuple(tabButton("Document"), Gtk::Overlay(), std::shared_ptr<document>(new document(this))));
		mDocumentIndexer->append_page(std::get<1>(mDocuments.back()), std::get<0>(mDocuments.back()));
		mDocumentIndexer->set_tab_reorderable(std::get<1>(mDocuments.back()));
		std::get<1>(mDocuments.back()).add(*std::get<2>(mDocuments.back())); // Add the document (for Gtk it is a box with a GLArea) to the new overlay
		mCurrentDocument = std::get<2>(mDocuments.back());
		if(mNavigationBar) {
			std::get<1>(mDocuments.back()).add_overlay(*mNavigationBar);
		} else {
			LOG_ERROR("Could not build navigation bar");
		}
	}
	
	if(mUI_upperBar) {
		mUI_upperBar->add(*mWorkspaces["sketchDesign"]->upperBar());
		mUI_upperBar->add(*mWorkspaces["partDesign"]->upperBar());
		mUI_upperBar->set_visible_child(*mWorkspaces["partDesign"]->upperBar());
		mCurrentWorkspace = mWorkspaces.at("partDesign");
		if(mNavigationBar)
			mNavigationBar->set_workspace(mCurrentWorkspace);
	}

	connect_signals();
	show_all();	
}

void bloop::set_workspace(std::string const& name, std::shared_ptr<entity> target)
{
	std::cout<<__FILE__<<",  "<<__LINE__<<" : Disabled\n";
	// if(mWorkspaces.find(name) != mWorkspaces.end()) {
	// 	mCurrentWorkspace = mWorkspaces.at(name);
	// 	mCurrentWorkspace->set_target(target);
	// }
}

void bloop::set_workspaceState(std::shared_ptr<workspaceState> state)
{
//	std::cout<<__FILE__<<",  "<<__LINE__<<" : Disabled\n\n";
	if(mCurrentWorkspace) {
		mCurrentWorkspace->set_state(state);
 	} else {
		LOG_ERROR("There are no current workspace in bloop");
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
	if(mCurrentDocument) {
		bool success;
		glm::ivec3 sampledColor = mCurrentDocument->selection_buffer_at(glm::ivec2(event->x, event->y), success);
		if(success)
			std::cout<<"Selection: "<<glm::to_string(sampledColor)<<std::endl;
	}
	if(mCurrentWorkspace) {
		return mCurrentWorkspace->manage_mouse_move(event);
	}
	return true;
}
bool bloop::manage_mouse_scroll(GdkEventScroll* event)
{
	if(mCurrentWorkspace) {
		return mCurrentWorkspace->manage_mouse_scroll(event);
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

void bloop::set_tool(GdkEventKey* event)
{
	std::cout<<__FILE__<<",  "<<__LINE__<<" : Disabled\n";
	//std::string toolName;
	// if(std::get<1>(mCurrentWorkSpace)->invoke_from_key(event->keyval, toolName))
	// 	set_tool(toolName);
}
void bloop::set_tool(std::string const& name) 
{
	std::cout<<__FILE__<<",  "<<__LINE__<<" : Disabled\n";
	// std::cout<<"Attempting to set tool to \""<<name<<"\"\n";
	// if(mTools.find(name) != mTools.end()) {
	// 	std::get<1>(mCurrentTool)->finish();
	// 	mCurrentTool = std::make_pair(name, mTools[name]);
	// 	std::get<1>(mCurrentTool)->set_subject(mCurrentDocument->focused_entity());
	// } else {
	// 	LOG_WARNING("Attempted to set current tool to \"" + name + "\". There is no such tool.")
	// }
}

void bloop::connect_signals()
{
	add_events( Gdk::POINTER_MOTION_MASK
						| Gdk::BUTTON_PRESS_MASK
						| Gdk::BUTTON_RELEASE_MASK
						| Gdk::KEY_PRESS_MASK
						| Gdk::KEY_RELEASE_MASK
						| Gdk::STRUCTURE_MASK
						| Gdk::SCROLL_MASK);
	signal_key_press_event().connect(sigc::mem_fun(*this, &bloop::manage_key_press));
	signal_motion_notify_event().connect(sigc::mem_fun(*this, &bloop::manage_mouse_move));
	signal_scroll_event().connect(sigc::mem_fun(*this, &bloop::manage_mouse_scroll));
	signal_button_release_event().connect(sigc::mem_fun(*this, &bloop::manage_button_release));	
}