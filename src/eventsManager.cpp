
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
		bool cam_updated = doc->update_camera();

		if(cam_updated || doc->target()->require_redraw()/* || self->mRequire_redraw*/) {
			doc->viewport().queue_draw();
		}
	}
	return true;
}

test_eventsManager::test_eventsManager():
	mTestStep(-1),
	mTestStepDone(0),
	mLastAction(std::chrono::steady_clock::now())
{

}

bool test_eventsManager::manage_key_press(GdkEventKey* event)
{
	if(event->keyval == GDK_KEY_o) {
		mTestStep = 1;
	}
	return true;
}

bool test_eventsManager::manage_tick(const Glib::RefPtr<Gdk::FrameClock>& frame_clock)
{
	using namespace std::chrono;
	static int i = 0;

	return stimuli_eventsManager::manage_tick(frame_clock);
}

void test_eventsManager::do_test()
{
	if(mTestStep >= mTestStepDone) {
		document_ptr doc = mBloopWindow->currentDocument();
		if(!doc)
			LOG_ERROR("Invalid document");
		// switch(mTestStep) {
		// case 0:

			
		// }
		// if(mTestStep > 0 && duration_cast<milliseconds>(steady_clock::now() - mLastAction).count() > 100) {
		// 	mLastAction = steady_clock::now();
		// 	std::cout<<"test! "<<i++<<"\n";
		// }
	}
}