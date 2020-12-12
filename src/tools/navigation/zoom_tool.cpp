
#include "zoom_tool.hpp"

#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <utils/mathUtils.hpp>

zoom_tool::zoom_tool(workspace* env): 
	tool_abstract(env)
{
	// Attempt to load the cursor icon
	try {
		mCursor->windowCursor = Gdk::Cursor::create(Gdk::Display::get_default(), Gdk::Pixbuf::create_from_file("resources/textures/images/icons/navigation/zoom.png", 30, 30), 0, 0);
	} catch(const Glib::FileError& ex) {
		LOG_WARNING("Glib::FileError: " + ex.what());
	} catch(const Gdk::PixbufError& ex) {
		LOG_WARNING("Glib::PixbufError: " + ex.what());
	}
}

void zoom_tool::finish()
{
	is_zooming = false; // Reset
}
void zoom_tool::init()
{
	is_zooming = false; // Reset
}

bool zoom_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(mEnv->state() && event->state & GDK_BUTTON1_MASK) { // When left button is pressed
		glm::vec2 pos(event->x, -event->y);
		float delta = pos.y - prevPos.y; // Only the y movement is of interest
		if(is_zooming) {
			zoom(zoomStart, 0.02f * delta); // The movement is scaled arbitrarly, zoom around starting point
		} else {
			zoomStart = glm::vec2(	map((float)event->x, 0.0f, (float)mEnv->state()->doc->get_width(), 	-1.0f, 1.0f),
									map((float)event->y, (float)mEnv->state()->doc->get_height(), 0.0f, -1.0f, 1.0f));
			is_zooming = true; // Now zooming, position are recorded
		}
		prevPos = pos; // Pos recorded
	} else {
		is_zooming = false;
	}
	return true;
}
bool zoom_tool::manage_button_release(GdkEventButton* event)
{
	if(event->state & GDK_BUTTON1_MASK)
		finish();
	return true;
}
bool zoom_tool::manage_scroll(GdkEventScroll* event)
{
	if(mEnv->state()) {
		zoom(glm::vec2(	map((float)event->x, 0.0f, (float)mEnv->state()->doc->get_width(), 	-1.0f, 1.0f),
						map((float)event->y, (float)mEnv->state()->doc->get_height(), 0.0f, -1.0f, 1.0f)), -0.015f * event->delta_y); // Delta is scaled arbitrarly, zooming around scroll origin
	}
	return true;
}

void zoom_tool::zoom(glm::vec2 origin, float amount)
{
	// The goal of this function is to scale the model and translate it so that the scale origin appears fixed

	// The final scale of the zoom operation
	float scale = 1.0f + amount;
	mEnv->state()->cam->zoom() *= scale;
		
	glm::vec4 event_pos(origin.x, origin.y, 0.0f, 0.0f);

	glm::mat4 mvp = mEnv->state()->cam->mvp();
	glm::mat4 vp = mEnv->state()->cam->projection() * mEnv->state()->cam->view();

	// Find where the model's center end up on screen
	glm::vec4 pos_screen = mvp * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	pos_screen /= pos_screen.w;

	// Find how the up-right vectors of the camera map on the screen 
	glm::vec4 up_screen = vp * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	up_screen /= up_screen.w;
	glm::vec4 right_screen = vp * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	right_screen /= right_screen.w;

	// Calculate the right-up stretch when mapped (they are initially 1.0f long)
	float up_length = std::sqrt(up_screen.x * up_screen.x + up_screen.y * up_screen.y);
	float right_length = std::sqrt(right_screen.x * right_screen.x + right_screen.y * right_screen.y);

	// Distance between the scale origin and the model origin mapped on screen
	glm::vec4 delta = event_pos - pos_screen;

	// How much the model moves is a function of how much it stretches in each direction and the scale amount
	float translate_by_x = (delta.x * -amount) / right_length;
	float translate_by_y = (delta.y * -amount) / up_length;

	// Set the translation
	mEnv->state()->cam->transformation().translation += glm::vec3(translate_by_x, translate_by_y, 0.0f);
}