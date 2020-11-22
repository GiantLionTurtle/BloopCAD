
#include "document.hpp"
#include "bloop.hpp"
#include "graphics_utils/GLCall.hpp"

#include "entities/part.hpp"

document::document(bloop* parent) :
	mParentBloop(parent),
	mCurrentWorkspaceState(nullptr)
{
	connect_signals();

	mWorkspaceStates["partDesign"] 			= std::shared_ptr<workspaceState>(new workspaceState);
	mWorkspaceStates.at("partDesign")->cam 	= std::shared_ptr<camera>(new camera(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(20.0f), 1.0f));
	mWorkspaceStates.at("partDesign")->cam->sphere() += glm::vec2(M_PI_4, -M_PI_4);

	mWorkspaceStates.at("partDesign")->doc 	= this;
	mWorkspaceStates.at("partDesign")->workspaceName = "partDesign";

	mWorkspaceStates["sketchDesign"] 		= std::shared_ptr<workspaceState>(new workspaceState);
	mWorkspaceStates.at("sketchDesign")->cam = std::shared_ptr<camera>(new camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(45.0f), 1.0f));
	mWorkspaceStates.at("sketchDesign")->doc = this;
	mWorkspaceStates.at("sketchDesign")->workspaceName = "sketchDesign";
}

void document::do_realize()
{
	mViewPort.make_current();
	try{
		mViewPort.throw_if_error();

		GLCall(const GLubyte *renderer_type = glGetString(GL_RENDERER));
		GLCall(const GLubyte *version = glGetString(GL_VERSION));

		if(renderer_type && version) {
				std::cout << "Realizing GL context" << "\n\t" << renderer_type << "\n\t" << version << std::endl;
				std::cout << "Dimensions: " << get_width() << " x " << get_height() << std::endl;
		}

		mViewPort.set_has_depth_buffer(true);
		GLCall(glEnable(GL_MULTISAMPLE));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS)); 
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		mSelectionBuffer = std::shared_ptr<frameBuffer>(new frameBuffer(get_width(), get_height()));
		mPart = std::shared_ptr<part>(new part());

		set_workspace("partDesign");
		if(mParentBloop->currentWorkspace())
			mCurrentWorkspaceState->currentTool = mParentBloop->currentWorkspace()->defaultTool();
	} catch(const Gdk::GLError& gle) {
		LOG_ERROR("\ndomain: " + std::to_string(gle.domain()) + "\ncode: " + std::to_string(gle.code()) + "\nwhat: " + gle.what());
	}
}
void document::do_unrealize()
{
	mViewPort.make_current();

	try{
		mViewPort.throw_if_error();
	} catch(const Gdk::GLError& gle) {
		LOG_ERROR("\ndomain: " + std::to_string(gle.domain()) + "\ncode: " + std::to_string(gle.code()) + "\nwhat: " + gle.what());
	}
}
bool document::do_render(const Glib::RefPtr<Gdk::GLContext>& /* context */)
{
	GLCall(glClearColor(1.0, 0.6, 0.88, 1.0));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	int initialFrameBuffer;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &initialFrameBuffer); // TODO: check if this could change (if it does not, no need to do it every loop)

	mCurrentWorkspaceState->cam->update();

	if(mPart) {
		mPart->draw(mCurrentWorkspaceState->cam);

		mSelectionBuffer->bind();
		GLCall(glViewport(0, 0, get_width(), get_height()));
		GLCall(glClearColor(0.0, 0.0, 0.0, 1.0));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		mPart->draw_selection(mCurrentWorkspaceState->cam);
		
		mSelectionBuffer->unbind(initialFrameBuffer);
	}
	return true;
}
gboolean document::frame_callback(GtkWidget* widget, GdkFrameClock* frame_clock, gpointer data)
{
    document* self = (document*) data;
    self->mViewPort.queue_draw();

    return G_SOURCE_CONTINUE;
}

void document::connect_signals()
{
	add_events(	  Gdk::POINTER_MOTION_MASK
				| Gdk::BUTTON_PRESS_MASK
				| Gdk::BUTTON_RELEASE_MASK
				| Gdk::KEY_PRESS_MASK
				| Gdk::KEY_RELEASE_MASK
				| Gdk::STRUCTURE_MASK
				| Gdk::SCROLL_MASK
	);
	mViewPort.add_events( Gdk::POINTER_MOTION_MASK
						| Gdk::BUTTON_PRESS_MASK
						| Gdk::BUTTON_RELEASE_MASK
						| Gdk::KEY_PRESS_MASK
						| Gdk::KEY_RELEASE_MASK
						| Gdk::STRUCTURE_MASK
						| Gdk::SCROLL_MASK);

	mViewPort.signal_realize().connect(sigc::mem_fun(*this, &document::do_realize));
	mViewPort.signal_unrealize().connect(sigc::mem_fun(*this, &document::do_unrealize), false);
	mViewPort.signal_render().connect(sigc::mem_fun(*this, &document::do_render));
	gtk_widget_add_tick_callback((GtkWidget*) this->gobj(),	document::frame_callback, this, NULL); // Couldn't find a c++-y way to do it
	mViewPort.signal_key_press_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_key_press));
	mViewPort.signal_key_release_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_key_release));
	mViewPort.signal_scroll_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_mouse_scroll));
	mViewPort.signal_motion_notify_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_mouse_move));
	mViewPort.signal_button_press_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_button_press));
	mViewPort.signal_button_release_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_button_release));

	pack_end(mViewPort);

	mViewPort.set_can_focus(true);
	mViewPort.grab_focus();
	mViewPort.set_hexpand(true);
	mViewPort.set_vexpand(true);
}

bool document::set_workspace(std::string const& name) 
{
	if(mWorkspaceStates.find(name) != mWorkspaceStates.end()) {
		mCurrentWorkspaceState = mWorkspaceStates[name];
		mCurrentWorkspaceState->cam->aspectRatio().set((float)get_width() / (float)get_height());

		mParentBloop->set_workspace(name, mCurrentWorkspaceState);
		return true;
	}
	return false;
}

bool document::set_workspace() 
{
	if(mCurrentWorkspaceState && mWorkspaceStates.find(mCurrentWorkspaceState->workspaceName) != mWorkspaceStates.end()) {
		mCurrentWorkspaceState->cam->aspectRatio().set((float)get_width() / (float)get_height());
		mCurrentWorkspaceState->target = mPart;

		mParentBloop->set_workspace(mCurrentWorkspaceState->workspaceName, mCurrentWorkspaceState);
		return true;
	}
	return false;
}

void document::push_action(std::shared_ptr<action> to_push)
{
	if(mActionInd != mActionStackSize) {
		mActionStackSize = mActionInd;
	}
	if(mActionStack.size() > mActionInd) {
		mActionStack[mActionInd] = to_push;
	} else {
		mActionStack.push_back(to_push);
	}

	to_push->do_work();
	mActionInd++;
	mActionStackSize++;
}
void document::advance_action_index(unsigned int amount)
{
	for(unsigned int i = 0; i < amount; ++i) {
		if((mActionInd + 1) < mActionStackSize) {
			mActionStack[mActionInd++]->do_work();
		}
	}
}
void document::rewind_action_index(unsigned int amount)
{
	for(unsigned int i = 0; i < amount; ++i) {
		if((mActionInd) > 0) {
			mActionStack[mActionInd--]->undo_work();
		}
	}
}