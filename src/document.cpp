
#include "document.hpp"
#include "bloop.hpp"
#include "graphics_utils/GLCall.hpp"

#include "entities/part.hpp"

document::document(bloop* parent) :
	mParentBloop(parent),
	mCurrentWorkspaceState(nullptr)
{
	connect_signals();

	mWorkspaceStates["partDesign"] = std::shared_ptr<workspaceState>(new workspaceState);
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
		mEntities = std::shared_ptr<entitiesIndexer>(new entitiesIndexer());
		mSubject = std::shared_ptr<entity>(new part(mEntities));
		mEntities->add(mSubject);
		set_workspace("partDesign");

		mCurrentWorkspaceState = std::shared_ptr<workspaceState>(new workspaceState);
		mCurrentWorkspaceState->cam = camera::from_spherical_ptr(glm::vec3(3.0f, 0.785398, 0.955317), glm::vec3(0.0f, 0.0f, 0.0f), 100.0f, (float)get_width() / (float)get_height());
		mCurrentWorkspaceState->indexer = mEntities;
		mCurrentWorkspaceState->selectionBuffer = mSelectionBuffer;
		mCurrentWorkspaceState->width = get_width();
		mCurrentWorkspaceState->height = get_height();
		mParentBloop->set_workspaceState(mCurrentWorkspaceState);
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

	if(mSubject) {
		mSubject->draw(mCurrentWorkspaceState->cam);

		mSelectionBuffer->bind();
		GLCall(glViewport(0, 0, get_width(), get_height()));
		GLCall(glClearColor(0.0, 0.0, 0.0, 1.0));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		mSubject->draw_selection(mCurrentWorkspaceState->cam);
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
	mViewPort.add_events( Gdk::POINTER_MOTION_MASK
						| Gdk::BUTTON_PRESS_MASK
						| Gdk::BUTTON_RELEASE_MASK
						| Gdk::KEY_PRESS_MASK
						| Gdk::KEY_RELEASE_MASK
						| Gdk::STRUCTURE_MASK);

	mViewPort.signal_realize().connect(sigc::mem_fun(*this, &document::do_realize));
	mViewPort.signal_unrealize().connect(sigc::mem_fun(*this, &document::do_unrealize), false);
	mViewPort.signal_render().connect(sigc::mem_fun(*this, &document::do_render));
	gtk_widget_add_tick_callback((GtkWidget*) this->gobj(),	document::frame_callback, this, NULL); // Couldn't find a c++-y way to do it
	
	// mViewPort.signal_key_press_event().connect(sigc::mem_fun(*mParentBloop, &bloop::manage_key_press));

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
		return true;
	}
	return false;
}

glm::ivec3 document::selection_buffer_at(glm::ivec2 pos, bool& success) 
{
	if(mSelectionBuffer) {
		mSelectionBuffer->bind();
		unsigned char data[4] = {0, 0, 0, 0};
		GLCall(glReadPixels(pos.x, get_height() - pos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data));
		success = true;
		return glm::ivec3(data[0], data[1], data[2]);
	}
	success = false;
	return glm::ivec3(0, 0, 0);
}