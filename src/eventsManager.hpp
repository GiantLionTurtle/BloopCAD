
#ifndef EVENTSMANAGER_HPP_
#define EVENTSMANAGER_HPP_

#include <forward_bloop.hpp>

#include <gtkmm.h>

#include <chrono>

class eventsManager {
protected:
	bloop* mBloopWindow;
public:
	eventsManager();

	void set_bloop(bloop* blp) { mBloopWindow = blp; }

	/*
		@function manage_key_press takes the current document's key press event
		and redirects it to the current workspace

		@param event : The key event handed by gtk
	*/
	virtual bool manage_key_press(GdkEventKey* event) = 0;
	/*
		@function manage_key_release takes the current document's key release event
		and redirects it to the current workspace

		@param event : The key event handed by gtk
	*/
	virtual bool manage_key_release(GdkEventKey* event) = 0;
	/*
		@function manage_mouse_move takes the current document's motion event
		and redirects it to the current workspace

		@param event : The motion event handed by gtk
	*/	
	virtual bool manage_mouse_move(GdkEventMotion* event) = 0;
	/*
		@function manage_mouse_scroll takes the current document's scroll event
		and redirects it to the current workspace

		@param event : The scroll event handed by gtk
	*/	
	virtual bool manage_mouse_scroll(GdkEventScroll* event) = 0;
	/*
		@function manage_mouse_scroll_internal keeps track of the scroll within the window
		because the scroll event captured by the document doesn't have a delta y somewhy

		@param event : The scroll event handed by gtk
	*/
	virtual bool manage_button_press(GdkEventButton* event) = 0;
	/*
		@function manage_button_release takes the current document's button release event
		and redirects it to the current workspace

		@param event : The button event handed by gtk
	*/	
	virtual bool manage_button_release(GdkEventButton* event) = 0;

	virtual bool manage_tick(const Glib::RefPtr<Gdk::FrameClock>& frame_clock) = 0;
};

class stimuli_eventsManager : public eventsManager {
private:

public:
	stimuli_eventsManager();

	virtual bool manage_key_press(GdkEventKey* event);
	virtual bool manage_key_release(GdkEventKey* event);
	virtual bool manage_mouse_move(GdkEventMotion* event);
	virtual bool manage_mouse_scroll(GdkEventScroll* event);
	virtual bool manage_button_press(GdkEventButton* event);
	virtual bool manage_button_release(GdkEventButton* event);

	virtual bool manage_tick(const Glib::RefPtr<Gdk::FrameClock>& frame_clock);
};

#endif