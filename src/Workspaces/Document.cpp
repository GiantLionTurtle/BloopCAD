
#include "Document.hpp"

#include <Utils/Expunge.hpp>
#include <Utils/Preferences.hpp>
#include "Bloop.hpp"
#include "Graphics_utils/GLCall.hpp"

Document::Document(EventsManager* manager):
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
	mName("Unamed doc"),
	mPart(nullptr),
	mToolPreview(nullptr),
	mVerboseLevel(0)
{
	if(!mEventsManager) {
		mEventsManager = new stimuli_EventsManager();
	}

	mViewport.set_required_version(3, 2);
	connect_signals();	
	// Create the workspace states, their Cameras and all
	mPartState 		= new WorkspaceState;
	mPartState->cam = new Camera(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(20.0f), glm::vec2(1.0f, 1.0f));
	mPartState->cam->set_orientation(glm::vec3(0.615480037895f, -M_PI_4, 0.0f));

	mPartState->doc 			= this;
	mPartState->workspaceName 	= Bloop::workspace_types::PART;

	mSketchState 		= new WorkspaceState;
	mSketchState->cam 	= new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(20.0f), glm::vec2(1.0f, 1.0f));
	mSketchState->doc 	= this;
	mSketchState->workspaceName = Bloop::workspace_types::SKETCH;

	// Now any part of the program can change the background color hehehe
	mBackgroundColor = Preferences::get_instance().get_vec3("background");
	Preferences::get_instance().add_callback("background", [this](glm::vec3 color) { mBackgroundColor = color; });
	mSideBar = new entityView(this);
}
Document::~Document()
{
	expunge(mSketchState);
	expunge(mPartState);
	
	expunge(mEventsManager);
	expunge(mSideBar);
	expunge(mPart);
}

void Document::make_glContext_current()
{
	mViewport.make_current(); // It's the viewport's context that is interesting
}

void Document::do_realize()
{
	make_glContext_current();
	try{
		mViewport.throw_if_error();

		// Check renderer
		GLCall(const GLubyte *renderer_type = glGetString(GL_RENDERER));
		GLCall(const GLubyte *version = glGetString(GL_VERSION));
		if(renderer_type && version) {
			verbose(VERBOSE_CONFIGS, "Realizing GL context" << "\n\t" << renderer_type << "\n\t" << version);
			verbose(VERBOSE_CONFIGS, "Dimensions: " << get_width() << " x " << get_height());
		}

		// Set up opengl
		mViewport.set_has_depth_buffer(true);
		GLCall(glEnable(GL_MULTISAMPLE));
		// GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS)); 
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		
		// Create an empty part. This surely should be in the constructor right?
		mPart = new Part();
		mPart->init();

		// Start with the part design workspace
		set_workspace(Bloop::workspace_types::PART);
		// Setyp tools and all
		if(mParentBloop->currentWorkspace())
			mCurrentWorkspaceState->currentTool = mParentBloop->currentWorkspace()->defaultTool();
		mPart->set_handle(new entityHandle(mPart, mSideBar, &mSideBar->root()));
		mParentBloop->add_sideBar(mSideBar);
		mParentBloop->set_sideBar(mSideBar);
		mParentBloop->notify_set_tool(mCurrentWorkspaceState->currentTool->name());
	} catch(const Gdk::GLError& gle) {
		LOG_ERROR("\ndomain: " + std::to_string(gle.domain()) + "\ncode: " + std::to_string(gle.code()) + "\nwhat: " + gle.what());
	}
}
void Document::do_unrealize()
{
	make_glContext_current();
	try {
		mViewport.throw_if_error();
	} catch(const Gdk::GLError& gle) {
		LOG_ERROR("\ndomain: " + std::to_string(gle.domain()) + "\ncode: " + std::to_string(gle.code()) + "\nwhat: " + gle.what());
	}
}
void Document::do_resize(int width, int height)
{
	if(mCurrentWorkspaceState)
		mCurrentWorkspaceState->cam->set_viewport(glm::vec2((float)width, (float)height)); // The dimensions might have changed, who knows?
}
bool Document::do_render(const Glib::RefPtr<Gdk::GLContext>& /* context */)
{
	mNeed_redraw = false;
	// Background draw
	GLCall(glViewport(0, 0, get_width(), get_height()));
	GLCall(glClearColor(mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, 1.0));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glDisable(GL_DEPTH_TEST)); // Disable the depth buffer to draw the whole quad, even if it is hidden by another semi-transparent quad	

	mPart->update();
	// if(mPart->need_redraw())
		// mPart->draw(mCurrentWorkspaceState->cam, mFrameId); // the other draw types are not used anywhere yet
	mPart->draw(mCurrentWorkspaceState->cam, mFrameId, Drawable::draw_type::ALL);
	mPart->draw(mCurrentWorkspaceState->cam, mFrameId, Drawable::draw_type::ACTIVE);

	if(mToolPreview) {
		mToolPreview->init(); // Will only actually init if it is not inited
		mToolPreview->update();
		mToolPreview->draw(mCurrentWorkspaceState->cam, mFrameId);
	}
	mFrameId++;
	return true;
}

void Document::connect_signals()
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

	// Standard things needed to init and render the Document
	mViewport.signal_realize().connect(sigc::mem_fun(*this, &Document::do_realize));
	mViewport.signal_unrealize().connect(sigc::mem_fun(*this, &Document::do_unrealize), false);
	mViewport.signal_resize().connect(sigc::mem_fun(*this, &Document::do_resize));
	mViewport.signal_render().connect(sigc::mem_fun(*this, &Document::do_render));

	mViewport.signal_key_press_event().connect(sigc::mem_fun(*mEventsManager, &EventsManager::manage_key_press));
	mViewport.signal_key_release_event().connect(sigc::mem_fun(*mEventsManager, &EventsManager::manage_key_release));
	mViewport.signal_scroll_event().connect(sigc::mem_fun(*mEventsManager, &EventsManager::manage_mouse_scroll));
	mViewport.signal_motion_notify_event().connect(sigc::mem_fun(*mEventsManager, &EventsManager::manage_mouse_move));
	mViewport.signal_button_press_event().connect(sigc::mem_fun(*mEventsManager, &EventsManager::manage_button_press));
	mViewport.signal_button_release_event().connect(sigc::mem_fun(*mEventsManager, &EventsManager::manage_button_release));
	mViewport.add_tick_callback(sigc::mem_fun(*mEventsManager, &EventsManager::manage_tick));

	pack_end(mViewport);

	// Take all the place please
	mViewport.set_can_focus(true);
	mViewport.grab_focus();
	mViewport.set_hexpand(true);
	mViewport.set_vexpand(true);
}

Workspace_abstr* Document::set_workspace(int name) 
{
	switch(name) {
	case Bloop::workspace_types::SKETCH:
		mCurrentWorkspaceState = mSketchState;
		break;
	case Bloop::workspace_types::PART:
		mCurrentWorkspaceState = mPartState;
		mParentBloop->partWorkspace()->set_target(mPart);
		break;
	default:
		LOG_WARNING("Trying to set workspace state to \"" + std::to_string(name) + "\". No such workspace state exist.");
		return nullptr;
	}
	mCurrentWorkspaceState->cam->set_viewport(glm::vec2((float)get_width(), (float)get_height())); // The dimensions might have changed, who knows?
	mSideBar->set_WorkspaceState(mCurrentWorkspaceState);
		
	return mParentBloop->set_workspace(name, mCurrentWorkspaceState); // Enforce change of workspace
}

Workspace_abstr* Document::set_workspace() 
{
	if(mCurrentWorkspaceState)
		return set_workspace(mCurrentWorkspaceState->workspaceName);
	return nullptr;
}
bool Document::has_workspace(int name)
{
	if(name == Bloop::workspace_types::SKETCH || name == Bloop::workspace_types::PART)
		return true;
	return false;
}

void Document::set_parent(Bloop* parentBloop)
{
	if(parentBloop) {
		mParentBloop = parentBloop;
		mEventsManager->set_bloop(parentBloop);
	} else {
		LOG_WARNING("Attempting to assign a null Bloop as parent.");
	}
}

bool Document::update_Camera()
{
	if(mCurrentWorkspaceState->cam->need_update()) {
		mCurrentWorkspaceState->cam->update();
		mCurrentCamState = mCurrentWorkspaceState->cam->state();
		return true;
	}
	return false;
}

void Document::push_action(Action_ptr to_push)
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
		verbose(VERBOSE_STEPS, "Done action");
		mCurrentActionNum++;
	}
	// Housekeeping incrementations
	mActionInd++;
	mActionStackSize++;
}
void Document::advance_action_index(unsigned int amount)
{
	mActionInd += amount;
	if(mActionInd >= mActionStackSize)
		mActionInd = mActionStackSize;
}
void Document::rewind_action_index(unsigned int amount)
{
	mActionInd -= amount;
	if(mActionInd < 0)
		mActionInd = 0;
}

void Document::update_actionStack()
{
	if(mCurrentActionNum < mActionInd) {
		if(mActionStack.at(mCurrentActionNum)->do_work(this)) {
			mCurrentActionNum++;
			verbose(VERBOSE_STEPS, "Done action");
		}
	} else if(mCurrentActionNum > mActionInd) {
		if(mActionStack.at(mCurrentActionNum-1)->undo_work(this)) {
			mCurrentActionNum--;
			verbose(VERBOSE_STEPS, "Undone action");
		}
	}
}

selection Document::selection_at(unsigned int ind)
{
	if(ind < selection_size()) {
		return mSelection.at(ind);
	}
	return {nullptr, {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)}}; // Empty selection
}
int Document::selection_ind(Drawable* ent)
{
	for(int i = 0; i < mSelection.size(); ++i) { // Linear search because I don't see huge selections coming, but it might change
		if(mSelection.at(i).ent == ent)
			return i;
	}
	return -1;
}
void Document::clear_selection()
{
	for(int i = 0; i < mSelection.size(); ++i) {
		mSelection.at(i).ent->unselect();
	}
	mSelection.clear();
}
void Document::toggle_select(Drawable* ent, CameraState cam, bool additive)
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
void Document::toggle_select(Drawable* ent, bool additive)
{
	toggle_select(ent, mCurrentCamState, additive);
}