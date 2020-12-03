
#include "startSketch_tool.hpp"

#include <preferences.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <entities/part.hpp>

#include <actions/partDesign/createSketch_action.hpp>
#include <actions/switchWorkspace_action.hpp>
#include <glm/gtx/vector_angle.hpp>

startSketch_tool::startSketch_tool(workspace* env):
	planeSelector_tool(env)
{
	try {
		mCursor->windowCursor = Gdk::Cursor::create(Gdk::Display::get_default(), Gdk::Pixbuf::create_from_file("resources/textures/images/icons/partDesign/cursors/startSketch_cursor.png", 16, 24), 0, 0);
	} catch(const Glib::FileError& ex) {
		LOG_WARNING("Glib::FileError: " + ex.what());
	} catch(const Gdk::PixbufError& ex) {
		LOG_WARNING("Glib::PixbufError: " + ex.what());
	}
}

void startSketch_tool::init()
{
	if(mEnv->state()) {
		if(mEnv->state()->doc->selection_size() == 1) {
			selection sel = mEnv->state()->doc->selection_at(0);
			std::shared_ptr<plane_abstract> sketchPlane = std::dynamic_pointer_cast<plane_abstract>(sel.ent);
			if(sketchPlane) {
				start_sketch(sketchPlane, sel.cam.up, sel.cam.right);
			}
		}
	}
}

bool startSketch_tool::manage_button_press(GdkEventButton* event)
{
	if(mEnv->state()) {
		std::shared_ptr<plane_abstract> sketchPlane = std::dynamic_pointer_cast<plane_abstract>(entity_at_point(glm::vec2(event->x, mEnv->state()->doc->get_height() - event->y)));
		std::cout<<"Startsketch cam: "<<glm::to_string(mEnv->state()->cam->pos())<<"\n";
		if(sketchPlane) {
			start_sketch(sketchPlane, mEnv->state()->cam->up(), mEnv->state()->cam->right());
		}
	}
	return true;
}

void startSketch_tool::start_sketch(std::shared_ptr<plane_abstract> sketchPlane, glm::vec3 camUp, glm::vec3 camRight)
{
	std::shared_ptr<part> target = std::dynamic_pointer_cast<part>(mEnv->state()->target);
	if(target) {
		mEnv->state()->doc->push_action(std::shared_ptr<action>(new createSketch_action(sketchPlane, target)));
		mEnv->state()->doc->push_action(std::shared_ptr<action>(new switchWorkspace_action(mEnv->state()->doc, "sketchDesign")));
		std::shared_ptr<workspaceState> newState = mEnv->state()->doc->currentWorkspaceState();

		newState->cam->set(mEnv->state()->cam);
		newState->target = target->get_sketch();

		float dot_right_v 	= glm::dot(camRight, sketchPlane->v());
		float dot_up_v 		= glm::dot(camUp, sketchPlane->v());

		glm::vec3 right_plane, up_plane;

		if(std::abs(dot_right_v) < std::abs(dot_up_v)) {
			right_plane = sketchPlane->w() * glm::sign(glm::dot(camRight, sketchPlane->w()));
			up_plane = sketchPlane->v() * glm::sign(dot_up_v);
		} else {
			right_plane = sketchPlane->v() * glm::sign(dot_right_v);
			up_plane = sketchPlane->w() * glm::sign(glm::dot(camUp, sketchPlane->w()));
		}
		newState->cam->go_to(sketchPlane->origin(), up_plane, right_plane, preferences::get_instance().get_long("camtrans"));
		newState->doc->clear_selection();
	}
}