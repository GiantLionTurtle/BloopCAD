
#include "document.hpp"

#include <utils/preferences.hpp>
#include "bloop.hpp"
#include "graphics_utils/GLCall.hpp"

document::document(eventsManager* manager):
	mParentBloop(),
	mEventsManager(manager),
	mCurrentWorkspaceState(nullptr),
	mBackgroundColor(0.0f, 0.0f, 0.0f),
	mFrameId(1),
	mActionStackSize(0),
	mActionInd(0),
	mCurrentActionNum(0),
	mNeed_redraw(false),
	mUseSelectionBuffer(true),
	mName("Unamed doc")
{
	if(!mEventsManager) {
		mEventsManager = new stimuli_eventsManager();
	}

	mViewport.set_required_version(3, 2);
	connect_signals();	
	// Create the workspace states, their Cameras and all
	mPartState 		= workspaceState_ptr(new workspaceState);
	mPartState->cam = Camera_ptr(new Camera(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(20.0f), glm::vec2(1.0f, 1.0f)));
	mPartState->cam->set_orientation(glm::vec3(0.615480037895f, -M_PI_4, 0.0f));

	mPartState->doc 			= this;
	mPartState->workspaceName 	= bloop::workspace_types::PART;

	mSketchState 		= workspaceState_ptr(new workspaceState);
	mSketchState->cam = Camera_ptr(new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(20.0f), glm::vec2(1.0f, 1.0f)));
	mSketchState->doc = this;
	mSketchState->workspaceName = bloop::workspace_types::SKETCH;

	// Now any part of the program can change the background color hehehe
	mBackgroundColor = preferences::get_instance().get_vec3("background");
	preferences::get_instance().add_callback("background", [this](glm::vec3 color) { mBackgroundColor = color; });
	mSideBar = new entityView(this);
}
document::~document()
{
	delete mEventsManager;
}

void document::make_glContext_current()
{
	mViewport.make_current(); // It's the viewport's context that is interesting
}

void document::do_realize()
{
	make_glContext_current();
	try{
		mViewport.throw_if_error();

		// Check renderer
		GLCall(const GLubyte *renderer_type = glGetString(GL_RENDERER));
		GLCall(const GLubyte *version = glGetString(GL_VERSION));
		if(renderer_type && version) {
				std::cout << "Realizing GL context" << "\n\t" << renderer_type << "\n\t" << version << std::endl;
				std::cout << "Dimensions: " << get_width() << " x " << get_height() << std::endl;
		}

		// Set up opengl
		mViewport.set_has_depth_buffer(true);
		GLCall(glEnable(GL_MULTISAMPLE));
		// GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS)); 
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		
		// Create an empty part. This surely should be in the constructor right?
		mPart = std::shared_ptr<part>(new part());

		// Start with the part design workspace
		set_workspace(bloop::workspace_types::PART);
		// Setyp tools and all
		if(mParentBloop->currentWorkspace())
			mCurrentWorkspaceState->currentTool = mParentBloop->currentWorkspace()->defaultTool();
		mPart->set_handle(new entityHandle(mPart, mSideBar, &mSideBar->root()));
		mParentBloop->add_sideBar(mSideBar);
		mParentBloop->set_sideBar(mSideBar);
		mParentBloop->notify_set_tool(mCurrentWorkspaceState->currentTool->name());

		// Setup target
		mCurrentWorkspaceState->target = mPart;
	} catch(const Gdk::GLError& gle) {
		LOG_ERROR("\ndomain: " + std::to_string(gle.domain()) + "\ncode: " + std::to_string(gle.code()) + "\nwhat: " + gle.what());
	}
}
void document::do_unrealize()
{
	make_glContext_current();
	try {
		mViewport.throw_if_error();
	} catch(const Gdk::GLError& gle) {
		LOG_ERROR("\ndomain: " + std::to_string(gle.domain()) + "\ncode: " + std::to_string(gle.code()) + "\nwhat: " + gle.what());
	}
}
void document::do_resize(int width, int height)
{
	if(mCurrentWorkspaceState)
		mCurrentWorkspaceState->cam->set_viewport(glm::vec2((float)width, (float)height)); // The dimensions might have changed, who knows?
}
bool document::do_render(const Glib::RefPtr<Gdk::GLContext>& /* context */)
{
	mNeed_redraw = false;
	// Background draw
	GLCall(glViewport(0, 0, get_width(), get_height()));
	GLCall(glClearColor(mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, 1.0));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glDisable(GL_DEPTH_TEST)); // Disable the depth buffer to draw the whole quad, even if it is hidden by another semi-transparent quad	
	if(mPart) {
		mPart->draw(mCurrentWorkspaceState->cam, mFrameId, Drawable::draw_type::INACTIVE);
		mPart->draw(mCurrentWorkspaceState->cam, mFrameId, Drawable::draw_type::ACTIVE);
	}
	mFrameId++;
	return true;
}

void document::connect_signals()
{
	// Is the double add_events really needed??
	add_events(	  Gdk::POINTER_MOTION_MASK
				| Gdk::BUTTON_PRESS_MASK
				| Gdk::BUTTON_RELEASE_MASK
				| Gdk::KEY_PRESS_MASK
				| Gdk::KEY_RELEASE_MASK
				| Gdk::STRUCTURE_MASK
				| Gdk::SCROLL_MASK
	);
	mViewport.add_events( Gdk::POINTER_MOTION_MASK
						| Gdk::BUTTON_PRESS_MASK
						| Gdk::BUTTON_RELEASE_MASK
						| Gdk::KEY_PRESS_MASK
						| Gdk::KEY_RELEASE_MASK
						| Gdk::STRUCTURE_MASK
						| Gdk::SCROLL_MASK);

	// Standard things needed to init and render the document
	mViewport.signal_realize().connect(sigc::mem_fun(*this, &document::do_realize));
	mViewport.signal_unrealize().connect(sigc::mem_fun(*this, &document::do_unrealize), false);
	mViewport.signal_resize().connect(sigc::mem_fun(*this, &document::do_resize));
	mViewport.signal_render().connect(sigc::mem_fun(*this, &document::do_render));

	mViewport.signal_key_press_event().connect(sigc::mem_fun(*mEventsManager, &eventsManager::manage_key_press));
	mViewport.signal_key_release_event().connect(sigc::mem_fun(*mEventsManager, &eventsManager::manage_key_release));
	mViewport.signal_scroll_event().connect(sigc::mem_fun(*mEventsManager, &eventsManager::manage_mouse_scroll));
	mViewport.signal_motion_notify_event().connect(sigc::mem_fun(*mEventsManager, &eventsManager::manage_mouse_move));
	mViewport.signal_button_press_event().connect(sigc::mem_fun(*mEventsManager, &eventsManager::manage_button_press));
	mViewport.signal_button_release_event().connect(sigc::mem_fun(*mEventsManager, &eventsManager::manage_button_release));
	mViewport.add_tick_callback(sigc::mem_fun(*mEventsManager, &eventsManager::manage_tick));

	pack_end(mViewport);

	// Take all the place please
	mViewport.set_can_focus(true);
	mViewport.grab_focus();
	mViewport.set_hexpand(true);
	mViewport.set_vexpand(true);
}

workspace_ptr document::set_workspace(int name) 
{
	switch(name) {
	case bloop::workspace_types::SKETCH:
		mCurrentWorkspaceState = mSketchState;
		break;
	case bloop::workspace_types::PART:
		mCurrentWorkspaceState = mPartState;
		mParentBloop->partWorkspace()->set_target(mPart);
		break;
	default:
		LOG_WARNING("Trying to set workspace state to \"" + std::to_string(name) + "\". No such workspace state exist.");
		return nullptr;
	}
	mCurrentWorkspaceState->cam->set_viewport(glm::vec2((float)get_width(), (float)get_height())); // The dimensions might have changed, who knows?
	mSideBar->set_workspaceState(mCurrentWorkspaceState);
		
	return mParentBloop->set_workspace(name, mCurrentWorkspaceState); // Enforce change of workspace
}

workspace_ptr document::set_workspace() 
{
	if(mCurrentWorkspaceState)
		return set_workspace(mCurrentWorkspaceState->workspaceName);
	return nullptr;
}
bool document::has_workspace(int name)
{
	if(name == bloop::workspace_types::SKETCH || name == bloop::workspace_types::PART)
		return true;
	return false;
}

void document::set_parent(bloop* parentBloop)
{
	if(parentBloop) {
		mParentBloop = parentBloop;
		mEventsManager->set_bloop(parentBloop);
	} else {
		LOG_WARNING("Attempting to assign a null bloop as parent.");
	}
}

bool document::update_Camera()
{
	if(mCurrentWorkspaceState->cam->require_update()) {
		mCurrentWorkspaceState->cam->update();
		mCurrentCamState = mCurrentWorkspaceState->cam->state();
		return true;
	}
	return false;
}

void document::push_action(std::shared_ptr<action> to_push)
{
	if(mActionInd != mActionStackSize) {
		mActionStackSize = mActionInd; // If an action is pushed while the index is in the past, it starts from there and erases the stuff going after
	}
	if(mActionStack.size() > mActionInd) { // No need to push if it is already allocated
		mActionStack[mActionInd] = to_push; 
	} else {
		mActionStack.push_back(to_push);
	}

	if(to_push->do_work(this)) {
		std::cout<<"Done action\n";
		mCurrentActionNum++;
	}
	// Housekeeping incrementations
	mActionInd++;
	mActionStackSize++;
}
void document::advance_action_index(unsigned int amount)
{
	mActionInd += amount;
	if(mActionInd >= mActionStackSize)
		mActionInd = mActionStackSize;
}
void document::rewind_action_index(unsigned int amount)
{
	mActionInd -= amount;
	if(mActionInd < 0)
		mActionInd = 0;
}

void document::update_actionStack()
{
	if(mCurrentActionNum < mActionInd) {
		if(mActionStack.at(mCurrentActionNum)->do_work(this)) {
			mCurrentActionNum++;
			std::cout<<"Done action\n";
		}
	} else if(mCurrentActionNum > mActionInd) {
		if(mActionStack.at(mCurrentActionNum-1)->undo_work(this)) {
			mCurrentActionNum--;
			std::cout<<"Undone action\n";
		}
	}
}

selection document::selection_at(unsigned int ind)
{
	if(ind < selection_size()) {
		return mSelection.at(ind);
	}
	return {nullptr, {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)}}; // Empty selection
}
int document::selection_ind(Drawable_ptr ent)
{
	for(int i = 0; i < mSelection.size(); ++i) { // Linear search because I don't see huge selections coming, but it might change
		if(mSelection.at(i).ent == ent)
			return i;
	}
	return -1;
}
void document::clear_selection()
{
	for(int i = 0; i < mSelection.size(); ++i) {
		mSelection.at(i).ent->unselect();
	}
	mSelection.clear();
}
void document::toggle_select(Drawable_ptr ent, CameraState cam, bool additive)
{
	if(!ent && !additive) {
		clear_selection();
		return;
	}

	int ind = selection_ind(ent);
	if(ind >= 0) { // The entity was already in the buffer
		if(additive) { // If additive, remove only the toggled entity
			mSelection.at(ind).ent->unselect(); // Notify the entity
			mSelection.erase(mSelection.begin() + ind); // Delete it
		} else { // If not additive, remove everything
			clear_selection();
		}
	} else {		
		if(!additive) { // If not additive, only the toggled entity should be selected ; the selection must be cleared
			clear_selection();
		}
		ent->select(); // Notify the entity
		mSelection.push_back(selection(ent, cam)); // Add it to the buffer
	}
}
void document::toggle_select(Drawable_ptr ent, bool additive)
{
	toggle_select(ent, mCurrentCamState, additive);
}