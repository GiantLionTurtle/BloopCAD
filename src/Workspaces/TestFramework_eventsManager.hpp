
#ifndef TESTFRAMEWORK_EVENTSMANAGER_HPP_
#define TESTFRAMEWORK_EVENTSMANAGER_HPP_

#include "EventsManager.hpp"

#include <Drawables/Sk/SkLine.hpp>
#include <Drawables/Sk/SkPoint.hpp>

class Test_EventsManager : public stimuli_EventsManager {
private:
	int mTestStep, mTestStepDone;
	SkPoint* mPoints[5];
	SkLine* mLines[4];
	std::chrono::steady_clock::time_point mLastAction;
public:
	Test_EventsManager();

	bool manage_key_press(GdkEventKey* event);

	bool manage_tick(const Glib::RefPtr<Gdk::FrameClock>& frame_clock);

	void do_test();
};

#endif