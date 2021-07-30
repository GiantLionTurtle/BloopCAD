
#include "eventsManager.hpp"

#include <bloop.hpp>
#include <document.hpp>

eventsManager::eventsManager()
{

}

stimuli_eventsManager::stimuli_eventsManager()
{

}

bool stimuli_eventsManager::manage_key_press(GdkEventKey* event)
{
	return mBloopWindow->currentWorkspace()->manage_key_press(event);
}
bool stimuli_eventsManager::manage_key_release(GdkEventKey* event)
{
	return mBloopWindow->currentWorkspace()->manage_key_release(event);
}
bool stimuli_eventsManager::manage_mouse_move(GdkEventMotion* event)
{
	return mBloopWindow->currentWorkspace()->manage_mouse_move(event);
}
bool stimuli_eventsManager::manage_mouse_scroll(GdkEventScroll* event)
{
	glm::vec2 deltas = mBloopWindow->scrollDeltas();
	event->delta_x = deltas.x;
	event->delta_y = deltas.y;
	return mBloopWindow->currentWorkspace()->manage_mouse_scroll(event);
}
bool stimuli_eventsManager::manage_button_press(GdkEventButton* event)
{
	return mBloopWindow->currentWorkspace()->manage_button_press(event);
}
bool stimuli_eventsManager::manage_button_release(GdkEventButton* event)
{
	return mBloopWindow->currentWorkspace()->manage_button_release(event);
}

bool stimuli_eventsManager::manage_tick(const Glib::RefPtr<Gdk::FrameClock>& frame_clock)
{
	document_ptr doc = mBloopWindow->currentDocument();

	if(doc) {
		doc->update_actionStack();
		bool cam_updated = doc->update_Camera();
		if(cam_updated || doc->target()->need_redraw()/* || self->mNeed_redraw*/) {
			doc->viewport().queue_draw();
		}
	}
	return true;
}