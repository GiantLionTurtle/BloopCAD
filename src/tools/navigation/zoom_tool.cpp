
#include "zoom_tool.hpp"

#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <utils/mathUtils.hpp>
#include <entities/geometry/plane_abstract.hpp>

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
			zoomStart = glm::vec2(event->x, event->y);
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
		zoom(glm::vec2(event->x, event->y), -0.015f * event->delta_y); // Delta is scaled arbitrarly, zooming around scroll origin
	}
	return true;
}

void zoom_tool::zoom(glm::vec2 origin, float amount)
{
	// The goal of this function is to scale the model and translate it so that the scale origin appears fixed
	float scale = 1.0f + amount;
	camera_ptr cam = mEnv->state()->cam;
	
	glm::vec3 ray1 = cam->cast_ray(origin, false);

	glm::vec4 model_pos1 = cam->model() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	plane_abstract pl1(glm::vec3(model_pos1), cam->right(), cam->up());
	glm::vec3 inter1 = pl1.line_intersection(cam->pos(), ray1);
	cam->set_scale(cam->transformation().scale * scale);

	glm::vec3 ray2 = cam->cast_ray(origin, false);

	glm::vec4 model_pos2 = cam->model() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	plane_abstract pl2(glm::vec3(model_pos2), cam->right(), cam->up());
	glm::vec3 inter2 = pl2.line_intersection(cam->pos(), ray2);
	
	cam->set_translation(cam->transformation().translation - (inter1-inter2) * cam->transformation().scale.x);
}