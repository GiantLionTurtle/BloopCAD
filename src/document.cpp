
#include "document.hpp"

#include <preferences.hpp>
#include "bloop.hpp"
#include "graphics_utils/GLCall.hpp"

#include "entities/part.hpp"

document::document(bloop* parent) :
	mParentBloop(parent),
	mCurrentWorkspaceState(nullptr),
	mBackgroundColor(0.0f, 0.0f, 0.0f)
{
	connect_signals();

	mWorkspaceStates["partDesign"] 			= std::shared_ptr<workspaceState>(new workspaceState);
	mWorkspaceStates.at("partDesign")->cam 	= std::shared_ptr<camera>(new camera(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(20.0f), 1.0f));
	mWorkspaceStates.at("partDesign")->cam->orientation() += glm::vec3(0.615480037895f, -M_PI_4, 0.0f);

	mWorkspaceStates.at("partDesign")->doc 	= this;
	mWorkspaceStates.at("partDesign")->workspaceName = "partDesign";

	mWorkspaceStates["sketchDesign"] 		= std::shared_ptr<workspaceState>(new workspaceState);
	mWorkspaceStates.at("sketchDesign")->cam = std::shared_ptr<camera>(new camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(45.0f), 1.0f));
	mWorkspaceStates.at("sketchDesign")->doc = this;
	mWorkspaceStates.at("sketchDesign")->workspaceName = "sketchDesign";

	mBackgroundColor = preferences::get_instance().get_vec3("background");
	preferences::get_instance().add_callback("background", [this](glm::vec3 color) { mBackgroundColor = color; });
}

void document::make_glContext_current()
{
	mViewport.make_current();
}

void document::do_realize()
{
	mViewport.make_current();
	try{
		mViewport.throw_if_error();

		GLCall(const GLubyte *renderer_type = glGetString(GL_RENDERER));
		GLCall(const GLubyte *version = glGetString(GL_VERSION));

		if(renderer_type && version) {
				std::cout << "Realizing GL context" << "\n\t" << renderer_type << "\n\t" << version << std::endl;
				std::cout << "Dimensions: " << get_width() << " x " << get_height() << std::endl;
		}

		mViewport.set_has_depth_buffer(true);
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
		mCurrentWorkspaceState->target = mPart;
	} catch(const Gdk::GLError& gle) {
		LOG_ERROR("\ndomain: " + std::to_string(gle.domain()) + "\ncode: " + std::to_string(gle.code()) + "\nwhat: " + gle.what());
	}
}
void document::do_unrealize()
{
	mViewport.make_current();

	try{
		mViewport.throw_if_error();
	} catch(const Gdk::GLError& gle) {
		LOG_ERROR("\ndomain: " + std::to_string(gle.domain()) + "\ncode: " + std::to_string(gle.code()) + "\nwhat: " + gle.what());
	}
}
bool document::do_render(const Glib::RefPtr<Gdk::GLContext>& /* context */)
{
	GLCall(glClearColor(mBackgroundColor.r, mBackgroundColor.g, mBackgroundColor.b, 1.0));
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
    self->mViewport.queue_draw();

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
	mViewport.add_events( Gdk::POINTER_MOTION_MASK
						| Gdk::BUTTON_PRESS_MASK
						| Gdk::BUTTON_RELEASE_MASK
						| Gdk::KEY_PRESS_MASK
						| Gdk::KEY_RELEASE_MASK
						| Gdk::STRUCTURE_MASK
						| Gdk::SCROLL_MASK);

	mViewport.signal_realize().connect(sigc::mem_fun(*this, &document::do_realize));
	mViewport.signal_unrealize().connect(sigc::mem_fun(*this, &document::do_unrealize), false);
	mViewport.signal_render().connect(sigc::mem_fun(*this, &document::do_render));
	gtk_widget_add_tick_callback((GtkWidget*) this->gobj(),	document::frame_callback, this, NULL); // Couldn't find a c++-y way to do it

	mViewport.signal_key_press_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_key_press));
	mViewport.signal_key_release_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_key_release));
	mViewport.signal_scroll_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_mouse_scroll));
	mViewport.signal_motion_notify_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_mouse_move));
	mViewport.signal_button_press_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_button_press));
	mViewport.signal_button_release_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_button_release));

	pack_end(mViewport);

	mViewport.set_can_focus(true);
	mViewport.grab_focus();
	mViewport.set_hexpand(true);
	mViewport.set_vexpand(true);
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
		// mCurrentWorkspaceState->target = mPart;

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

selection document::selection_at(unsigned int ind)
{
	if(ind < selection_size()) {
		return std::get<1>(mSelection.at(ind));
	}
	return {nullptr, {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)}};
}
int document::selection_ind(glm::ivec3 id)
{
	for(int i = 0; i < mSelection.size(); ++i) {
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
	if(ind >= 0) {
		if(additive) {
			std::get<1>(mSelection.at(ind)).ent->unselect();
			mSelection.erase(mSelection.begin() + ind);
		} else {
			clear_selection();
		}
	} else {
		std::shared_ptr<entity> ent = mPart->get(id);
		if(ent) {
			if(!additive) {
				clear_selection();
			}
			ent->select();
			mSelection.push_back(std::make_pair(id, selection(ent, cam)));
		} else {
			clear_selection();
		}
	}
}