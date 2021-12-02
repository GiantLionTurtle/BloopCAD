
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

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

#ifndef EVENTSMANAGER_HPP_
#define EVENTSMANAGER_HPP_

#include <gtkmm.h>
#include <chrono>

class Bloop;

class EventsManager {
protected:
	Bloop* mBloopWindow;
public:
	EventsManager();

	void set_bloop(Bloop* blp) { mBloopWindow = blp; }

	/*
		@function manage_key_press takes the current Document's key press event
		and redirects it to the current workspace

		@param event : The key event handed by gtk
	*/
	virtual bool manage_key_press(GdkEventKey* event) = 0;
	/*
		@function manage_key_release takes the current Document's key release event
		and redirects it to the current workspace

		@param event : The key event handed by gtk
	*/
	virtual bool manage_key_release(GdkEventKey* event) = 0;
	/*
		@function manage_mouse_move takes the current Document's motion event
		and redirects it to the current workspace

		@param event : The motion event handed by gtk
	*/	
	virtual bool manage_mouse_move(GdkEventMotion* event) = 0;
	/*
		@function manage_mouse_scroll takes the current Document's scroll event
		and redirects it to the current workspace

		@param event : The scroll event handed by gtk
	*/	
	virtual bool manage_mouse_scroll(GdkEventScroll* event) = 0;
	/*
		@function manage_mouse_scroll_internal keeps track of the scroll within the window
		because the scroll event captured by the Document doesn't have a delta y somewhy

		@param event : The scroll event handed by gtk
	*/
	virtual bool manage_button_press(GdkEventButton* event) = 0;
	/*
		@function manage_button_release takes the current Document's button release event
		and redirects it to the current workspace

		@param event : The button event handed by gtk
	*/	
	virtual bool manage_button_release(GdkEventButton* event) = 0;

	virtual bool manage_tick(const Glib::RefPtr<Gdk::FrameClock>& frame_clock) = 0;
};

class stimuli_EventsManager : public EventsManager {
private:

public:
	stimuli_EventsManager();

	virtual bool manage_key_press(GdkEventKey* event);
	virtual bool manage_key_release(GdkEventKey* event);
	virtual bool manage_mouse_move(GdkEventMotion* event);
	virtual bool manage_mouse_scroll(GdkEventScroll* event);
	virtual bool manage_button_press(GdkEventButton* event);
	virtual bool manage_button_release(GdkEventButton* event);

	virtual bool manage_tick(const Glib::RefPtr<Gdk::FrameClock>& frame_clock);
};

#endif