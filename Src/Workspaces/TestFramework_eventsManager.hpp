
// BloopCAD
// Copyright (C) 2020  BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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