
#include "document.hpp"

#include <preferences.hpp>
#include "bloop.hpp"
#include "graphics_utils/GLCall.hpp"

#include "entities/part.hpp"

document::document(bloop* parent) :
	mParentBloop(parent),
	mCurrentWorkspaceState(nullptr),
	mBackgroundColor(0.0f, 0.0f, 0.0f),
	mFrameId(1),
	mActionStackSize(0),
	mActionInd(0),
	mCurrentActionInd(0)
{
	connect_signals();	
	// Create the workspace states, their cameras and all
	mWorkspaceStates["partDesign"] 			= std::shared_ptr<workspaceState>(new workspaceState);
	mWorkspaceStates.at("partDesign")->cam 	= std::shared_ptr<camera>(new camera(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(20.0f), glm::vec2(1.0f, 1.0f)));
	mWorkspaceStates.at("partDesign")->cam->set_orientation(glm::vec3(0.615480037895f, -M_PI_4, 0.0f));

	mWorkspaceStates.at("partDesign")->doc 	= this;
	mWorkspaceStates.at("partDesign")->workspaceName = "partDesign";

	mWorkspaceStates["sketchDesign"] 		= std::shared_ptr<workspaceState>(new workspaceState);
	mWorkspaceStates.at("sketchDesign")->cam = std::shared_ptr<camera>(new camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(20.0f), glm::vec2(1.0f, 1.0f)));
	mWorkspaceStates.at("sketchDesign")->doc = this;
	mWorkspaceStates.at("sketchDesign")->workspaceName = "sketchDesign";

	// Now any part of the program can change the background color hehehe
	mBackgroundColor = preferences::get_instance().get_vec3("background");
	preferences::get_instance().add_callback("background", [this](glm::vec3 color) { mBackgroundColor = color; });
	mSideBar = new entityView(this);
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
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS)); 
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Init selection buffer
		mSelectionBuffer = std::shared_ptr<frameBuffer>(new frameBuffer(get_width(), get_height()));
		
		// Create an empty part. This surely should be in the constructor right?
		mPart = std::shared_ptr<part>(new part());

		// Start with the part design workspace
		set_workspace("partDesign");
		// Setyp tools and all
		if(mParentBloop->currentWorkspace())
			mCurrentWorkspaceState->currentTool = mParentBloop->currentWorkspace()->defaultTool();
		
		mPart->set_handle(new entityHandle(mPart, mSideBar, &mSideBar->root()));
		mParentBloop->add_sideBar(mSideBar);
		mParentBloop->set_sideBar(mSideBar);

		// Setup target
		mCurrentWorkspaceState->target = mPart;
	} catch(const Gdk::GLError& gle) {
		LOG_ERROR("\ndomain: " + std::to_string(gle.domain()) + "\ncode: " + std::to_string(gle.code()) + "\nwhat: " + gle.what());
	}
}
void document::do_unrealize()
{
	make_glContext_current();
	try{
		mViewport.throw_if_error();
	} catch(const Gdk::GLError& gle) {
		LOG_ERROR("\ndomain: " + std::to_string(gle.domain()) + "\ncode: " + std::to_string(gle.code()) + "\nwhat: " + gle.what());
	}
}
void document::do_resize(int width, int height)
{
	// TODO: manage selection frame buffer?
	if(mCurrentWorkspaceState)
		mCurrentWorkspaceState->cam->viewport() = glm::vec2((float)width, (float)height); // The dimensions might have changed, who knows?
}
bool document::do_render(const Glib::RefPtr<Gdk::GLContext>& /* context */)
{
	// Background draw
	GLCall(glViewport(0, 0, get_width(), get_height()));
	GLCall(glClearColor(mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, 1.0));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	
	// Record the rendering buffer because the selection buffer will be bound
	int initialFrameBuffer;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &initialFrameBuffer); // TODO: check if this could change (if it does not, no need to do it every loop)

	if(mPart) {
		GLCall(glDisable(GL_DEPTH_TEST)); // Disable the depth buffer to draw the whole quad, even if it is hidden by another semi-transparent quad

		// Draw the part
		mPart->draw(mCurrentWorkspaceState->cam, mFrameId);
		
		GLCall(glEnable(GL_DEPTH_TEST));

		// Draw the selection shadows of the part
		mSelectionBuffer->bind();
		GLCall(glViewport(0, 0, get_width(), get_height()));
		GLCall(glClearColor(0.0, 0.0, 0.0, 1.0));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		mPart->draw_selection(mCurrentWorkspaceState->cam, mFrameId);
		
		// Clean up
		mSelectionBuffer->unbind(initialFrameBuffer);
	}
	mFrameId++;
	return true;
}
gboolean document::frame_callback(GtkWidget* widget, GdkFrameClock* frame_clock, gpointer data)
{
    document* self = (document*) data;

	self->update_actionStack();
	camState cmSt = self->mCurrentWorkspaceState->cam->state();

	if(self->mCurrentCamState != cmSt || self->target()->require_redraw()) {
		self->mCurrentCamState = cmSt;
		self->mViewport.queue_draw();
	}

    return G_SOURCE_CONTINUE;
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
	gtk_widget_add_tick_callback((GtkWidget*) this->gobj(),	document::frame_callback, this, NULL); // Couldn't find a c++-y way to do it

	// Events happening on the viewport but handled by bloop, because it has direct access to the workspaces
	mViewport.signal_key_press_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_key_press));
	mViewport.signal_key_release_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_key_release));
	mViewport.signal_scroll_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_mouse_scroll));
	mViewport.signal_motion_notify_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_mouse_move));
	mViewport.signal_button_press_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_button_press));
	mViewport.signal_button_release_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_button_release));

	pack_end(mViewport);

	// Take all the place please
	mViewport.set_can_focus(true);
	mViewport.grab_focus();
	mViewport.set_hexpand(true);
	mViewport.set_vexpand(true);
}

std::shared_ptr<workspace> document::set_workspace(std::string const& name) 
{
	if(mWorkspaceStates.find(name) != mWorkspaceStates.end()) {
		mCurrentWorkspaceState = mWorkspaceStates[name];
		mCurrentWorkspaceState->cam->viewport() = glm::vec2((float)get_width(), (float)get_height()); // The dimensions might have changed, who knows?
		mSideBar->set_workspaceState(mCurrentWorkspaceState);
		return mParentBloop->set_workspace(name, mCurrentWorkspaceState); // Enforce change of workspace
	}
	return nullptr;
}

std::shared_ptr<workspace> document::set_workspace() 
{
	if(mCurrentWorkspaceState && mWorkspaceStates.find(mCurrentWorkspaceState->workspaceName) != mWorkspaceStates.end()) {
		mCurrentWorkspaceState->cam->viewport() = glm::vec2((float)get_width(), (float)get_height());
		mSideBar->set_workspaceState(mCurrentWorkspaceState);

		return mParentBloop->set_workspace(mCurrentWorkspaceState->workspaceName, mCurrentWorkspaceState); // Enforce workspace choice
	}
	return nullptr;
}
bool document::has_workspace(std::string const& name)
{
	if(mWorkspaceStates.find(name) != mWorkspaceStates.end())
		return true;
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

	if(to_push->do_work())
		mCurrentActionInd++;
	// Housekeeping incrementations
	mActionInd++;
	mActionStackSize++;
}
void document::advance_action_index(unsigned int amount)
{
	mActionInd += amount;
	if(mActionInd >= mActionStackSize)
		mActionInd = mActionStackSize-1;
}
void document::rewind_action_index(unsigned int amount)
{
	mActionInd -= amount;
	if(mActionInd < 0)
		mActionInd = 0;
}

void document::update_actionStack()
{
	if(mCurrentActionInd < mActionInd) {
		if(mActionStack[mCurrentActionInd]->do_work()) {
			mCurrentActionInd++;
			std::cout<<"Done action\n";
		}
	} else if(mCurrentActionInd > mActionInd) {
		if(mActionStack[mCurrentActionInd]->undo_work())
			mCurrentActionInd--;
	}
}

selection document::selection_at(unsigned int ind)
{
	if(ind < selection_size()) {
		return std::get<1>(mSelection.at(ind));
	}
	return {nullptr, {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)}}; // Empty selection
}
int document::selection_ind(glm::ivec3 id)
{
	for(int i = 0; i < mSelection.size(); ++i) { // Linear search because I don't see huge selections coming, but it might change
		if(std::get<0>(mSelection.at(i)) == id)
			return i;
	}
	return -1;
}
void document::clear_selection()
{
	for(int i = 0; i < mSelection.size(); ++i) {
		std::get<1>(mSelection.at(i)).ent->unselect();
	}
	mSelection.clear();
}
void document::toggle_select(glm::ivec3 id, camState cam, bool additive)
{
	int ind = selection_ind(id);
	if(ind >= 0) { // The entity was already in the buffer
		if(additive) { // If additive, remove only the toggled entity
			std::get<1>(mSelection.at(ind)).ent->unselect(); // Notify the entity
			mSelection.erase(mSelection.begin() + ind); // Delete it
		} else { // If not additive, remove everything
			clear_selection();
		}
	} else {
		std::shared_ptr<entity> ent = mPart->get(id); // Find the entity
		if(ent) {
			if(!additive) { // If not additive, only the toggled entity should be selected ; the selection must be cleared
				clear_selection();
			}
			ent->select(); // Notify the entity
			mSelection.push_back(std::make_pair(id, selection(ent, cam))); // Add it to the buffer
		} else if(!additive) { // Clicked on non existing entity (likely the background) ; clear the selection except if the additive mode is on
			clear_selection();
		}
	}
}

void document::toggle_select(std::shared_ptr<entity> ent, bool additive)
{
	toggle_select(ent->selectionID(), mCurrentCamState, additive);
}