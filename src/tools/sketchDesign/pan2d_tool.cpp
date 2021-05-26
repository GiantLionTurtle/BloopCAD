
#include "pan2d_tool.hpp"
#include <glm/gtx/vector_angle.hpp>

#include <workspaces/sketchDesign.hpp>
#include <document.hpp>

pan2d_tool::pan2d_tool(sketchDesign* env): 
	tool(env, compositeCursor_ptr(new compositeCursor
		{Gdk::Cursor::create(Gdk::IRON_CROSS), nullptr, glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)})) // Iron cross is the closest to the icon
{
	// Attempt to load cursor icon
	try {
		mCursor->windowCursor = Gdk::Cursor::create(Gdk::Display::get_default(), Gdk::Pixbuf::create_from_file("resources/textures/images/icons/navigation/pan.png", 30, 30), 0, 0);
	} catch(const Glib::FileError& ex) {
		LOG_WARNING("Glib::FileError: " + ex.what());
	} catch(const Gdk::PixbufError& ex) {
		LOG_WARNING("Glib::PixbufError: " + ex.what());
	}
}

bool pan2d_tool::manage_button_press(GdkEventButton* event)
{
	if(!event->state & GDK_BUTTON1_MASK)
		return true;
    camera_ptr cam = mEnv->state()->cam;
	mDragStart = dist_to_cam(glm::vec2(event->x, event->y));
	mTranStart = cam->transformation().translation;
	is_moving = true;
	return true;
}
bool pan2d_tool::manage_button_release(GdkEventButton* event)
{
	if(!event->state & GDK_BUTTON1_MASK)
		return true;
	is_moving = false;
	return true;
}
bool pan2d_tool::manage_mouse_move(GdkEventMotion* event) 
{
	if(mEnv->state() && event->state & GDK_BUTTON1_MASK) {
		if(is_moving) {
			camera_ptr cam = mEnv->state()->cam;
			glm::vec2 mov_2d = dist_to_cam(glm::vec2(event->x, event->y)) - mDragStart;
			glm::vec3 mov = mPl_right * mov_2d.x + mPl_up * mov_2d.y;
			cam->set_translation(mTranStart + mov, false); // Move the model (no need to get fancy, it moves according to the "real position of the camera")
		} else {
			is_moving = true; // Now moving, first point recorded
		}
	} else {
		is_moving = false; // Model no longer moving
	}
	return true;
}

glm::vec2 pan2d_tool::dist_to_cam(glm::vec2 mousepos)
{
	camera_ptr cam = mEnv->state()->cam;
	auto pl = mEnv->target()->basePlane();
	cam->get_alignedPlaneVectors(pl, mPl_right, mPl_up, false);
	float ang_cam_x = glm::orientedAngle(glm::normalize(cam->right()), glm::normalize(mPl_right), glm::normalize(mPl_up));
	float ang_cam_y = glm::orientedAngle(glm::normalize(cam->up()), glm::normalize(mPl_up), glm::normalize(mPl_right));
	float dist_cam_pl = pl->dist_to_point(cam->pos());
	glm::vec2 mouse_angle = cam->screen_angle(mousepos);
	float compound_angle_x = ang_cam_x + mouse_angle.x;
	float compound_angle_y = ang_cam_y + mouse_angle.y;
	float mov_x = std::tan(compound_angle_x) * dist_cam_pl;
	float mov_y = std::tan(compound_angle_y) * dist_cam_pl;

	return glm::vec2(mov_x, mov_y);
}