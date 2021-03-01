
#include "startSketch_tool.hpp"

#include <utils/preferences.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <entities/tangibleEntities/plane.hpp>
#include <actions/common/enableEntity_action.hpp>
#include <actions/sketchDesign/enterSketchDesign_action.hpp>
#include <actions/partDesign/quitPartDesign_action.hpp>
#include <actions/common/serial_action.hpp>
#include <actions/common/moveCamera_action.hpp>
#include <glm/gtx/vector_angle.hpp>

startSketch_tool::startSketch_tool(partDesign* env):
	simpleSelector_tool(env)
{
	// Attempt to load the cursor icon
	try {
		mCursor->windowCursor = Gdk::Cursor::create(Gdk::Display::get_default(), Gdk::Pixbuf::create_from_file("resources/textures/images/icons/partDesign/cursors/startSketch_cursor.png", 16, 24), 0, 0);
	} catch(const Glib::FileError& ex) {
		LOG_WARNING("Glib::FileError: " + ex.what());
	} catch(const Gdk::PixbufError& ex) {
		LOG_WARNING("Glib::PixbufError: " + ex.what());
	}

	mFilter = [](entity_ptr ent) -> bool { return std::dynamic_pointer_cast<geom_3d::plane_abstr>(ent).operator bool(); };
}

void startSketch_tool::init()
{
	if(mEnv->state()) {
		// Check if there is only one item in the document's selection stack and if it is a plane, use it
		if(mEnv->state()->doc->selection_size() == 1) {
			selection sel = mEnv->state()->doc->selection_at(0);
			geom_3d::plane_abstr_ptr sketchPlane = std::dynamic_pointer_cast<geom_3d::plane_abstr>(sel.ent);
			if(sketchPlane) {
				start_sketch(sketchPlane, sel.camSt);
				sel.ent->set_hover(false);
			}
		}
	}
}

bool startSketch_tool::manage_button_press(GdkEventButton* event)
{
	if(mEnv->state()) {
		// If the hovered entity is a plane, start sketch
		entity_ptr ent = entity_at_point(glm::vec2(event->x, event->y));
		geom_3d::plane_abstr_ptr sketchPlane = std::dynamic_pointer_cast<geom_3d::plane_abstr>(ent);
		if(sketchPlane) {
			start_sketch(sketchPlane, mEnv->state()->cam->state());
			ent->set_hover(false);
		}
	}
	return true;
}

void startSketch_tool::act_on_entity(entity_ptr ent)
{
	if(mEnv->state() && mFilter(ent))
		start_sketch(std::dynamic_pointer_cast<geom_3d::plane_abstr>(ent), mEnv->state()->cam->state());
}

void startSketch_tool::start_sketch(geom_3d::plane_abstr_ptr sketchPlane, cameraState const& camState_)
{
	mEnv->state()->doc->make_glContext_current();
	mCurrentSketch = std::make_shared<sketch>(sketchPlane);
	mEnv->target()->add_sketch(mCurrentSketch);
	mEnv->state()->doc->push_action(std::shared_ptr<serial_action>(new serial_action({
		std::shared_ptr<action>(new enableEntity_action(mCurrentSketch)),
		std::shared_ptr<action>(new enterSketchDesign_action(mCurrentSketch, true)),
		std::shared_ptr<action>(new quitPartDesign_action()),
		moveCamera_action::create_from_facingPlane(sketchPlane, 8.0, camState_, nullptr)
	})));
}