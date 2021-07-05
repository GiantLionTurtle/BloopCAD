
#ifndef TESTFRAMEWORK_EVENTSMANAGER_HPP_
#define TESTFRAMEWORK_EVENTSMANAGER_HPP_

#include "eventsManager.hpp"

#include <Drawables/tangibleEntities/sketchEntities/sketchPoint.hpp>
#include <Drawables/tangibleEntities/sketchEntities/sketchLine.hpp>

class test_eventsManager : public stimuli_eventsManager {
private:
	int mTestStep, mTestStepDone;
	sketchPoint_ptr mPoints[5];
	sketchLine_ptr mLines[4];
	std::chrono::steady_clock::time_point mLastAction;
public:
	test_eventsManager();

	bool manage_key_press(GdkEventKey* event);

	bool manage_tick(const Glib::RefPtr<Gdk::FrameClock>& frame_clock);

	void do_test();
};

#endif