
#include "startSketch_tool.hpp"

#include <utils/preferences.hpp>
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <entities/part.hpp>

#include <actions/partDesign/createSketch_action.hpp>
#include <actions/common/switchWorkspace_action.hpp>
#include <actions/common/moveCamera_action.hpp>
#include <glm/gtx/vector_angle.hpp>

startSketch_tool::startSketch_tool(workspace* env):
	planeSelector_tool(env)
{
	// Attempt to load the cursor icon
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
		// Check if there is only one item in the document's selection stack and if it is a plane, use it
		if(mEnv->state()->doc->selection_size() == 1) {
			selection sel = mEnv->state()->doc->selection_at(0);
			std::shared_ptr<plane_abstract> sketchPlane = std::dynamic_pointer_cast<plane_abstract>(sel.ent);
			if(sketchPlane) {
				start_sketch(sketchPlane, sel.camSt);
			}
		}
	}
}

bool startSketch_tool::manage_button_press(GdkEventButton* event)
{
	if(mEnv->state()) {
		// If the hovered entity is a plane, start sketch
		std::shared_ptr<plane_abstract> sketchPlane = std::dynamic_pointer_cast<plane_abstract>(entity_at_point(glm::vec2(event->x, mEnv->state()->doc->get_height() - event->y)));
		if(sketchPlane) {
			start_sketch(sketchPlane, mEnv->state()->cam->state());
		}
	}
	return true;
}

void startSketch_tool::act_on_entity(std::shared_ptr<entity> ent)
{
	if(mEnv->state() && entity_valid(ent))
		start_sketch(std::dynamic_pointer_cast<plane_abstract>(ent), mEnv->state()->cam->state());
}

void startSketch_tool::start_sketch(std::shared_ptr<plane_abstract> sketchPlane, camState const& camState_)
{
	std::shared_ptr<part> target = std::dynamic_pointer_cast<part>(mEnv->state()->target); // Aquire the part that is worked on
	if(target) {
		mEnv->state()->doc->push_action(std::shared_ptr<action>(new createSketch_action(sketchPlane, target)));
		mEnv->state()->doc->push_action(std::shared_ptr<action>(new switchWorkspace_action(mEnv->state()->doc, "sketchDesign")));
		std::shared_ptr<workspaceState> newState = mEnv->state()->doc->currentWorkspaceState(); // This tool is still owned by partDesign so it has to retrieve the sketchDesign workspace state

		newState->cam->set(mEnv->state()->cam); // Set sketch camera to part camera for seemless transition
		newState->target = target->get_sketch(); // Set the state's target to the new sketch

		newState->doc->clear_selection();
		mEnv->state()->doc->push_action(moveCamera_action::create_from_facingPlane(sketchPlane, 8.0, camState_, newState->cam));
	}
}