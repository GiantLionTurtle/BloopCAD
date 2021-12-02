
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

#include "EventsManager.hpp"

#include <Bloop/Bloop.hpp>
#include <Workspaces/Document.hpp>

#include <glm/glm.hpp>

EventsManager::EventsManager()
{

}

stimuli_EventsManager::stimuli_EventsManager()
{

}

bool stimuli_EventsManager::manage_key_press(GdkEventKey* event)
{
	return mBloopWindow->currentWorkspace()->manage_key_press(event);
}
bool stimuli_EventsManager::manage_key_release(GdkEventKey* event)
{
	return mBloopWindow->currentWorkspace()->manage_key_release(event);
}
bool stimuli_EventsManager::manage_mouse_move(GdkEventMotion* event)
{
	return mBloopWindow->currentWorkspace()->manage_mouse_move(event);
}
bool stimuli_EventsManager::manage_mouse_scroll(GdkEventScroll* event)
{
	glm::vec2 deltas = mBloopWindow->scrollDeltas();
	event->delta_x = deltas.x;
	event->delta_y = deltas.y;
	return mBloopWindow->currentWorkspace()->manage_mouse_scroll(event);
}
bool stimuli_EventsManager::manage_button_press(GdkEventButton* event)
{
	return mBloopWindow->currentWorkspace()->manage_button_press(event);
}
bool stimuli_EventsManager::manage_button_release(GdkEventButton* event)
{
	return mBloopWindow->currentWorkspace()->manage_button_release(event);
}

bool stimuli_EventsManager::manage_tick(const Glib::RefPtr<Gdk::FrameClock>& frame_clock)
{
	Document* doc = mBloopWindow->currentDocument();

	if(doc) {
		doc->update_actionStack();
		bool cam_updated = doc->update_Camera();
		if(cam_updated || doc->target()->need_redraw() || doc->need_redraw()) {
			doc->viewport().queue_draw();
		}
	}
	return true;
}