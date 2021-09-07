
#include "StartSketch_tool.hpp"

#include <document.hpp>
#include <Drawables/Containers/Sketch.hpp>
#include <Drawables/3d/Plane.hpp>
#include <Actions/Common/ToggleBaseObject_action.hpp>
#include <Actions/Sketch/EnterSketch_action.hpp>
#include <Actions/Part/QuitPartDesign_action.hpp>
#include <Actions/Common/Serial_action.hpp>
#include <Actions/Common/MoveCamera_action.hpp>

StartSketch_tool::StartSketch_tool(Part_ws* env):
	SimpleSelector_tool(env)
{
	load_icon("resources/textures/images/icons/partDesign/cursors/startSketch_cursor.png");
}

void StartSketch_tool::init()
{
	if(mEnv->state()) {
		// Check if there is only one item in the document's selection stack and if it is a plane, use it
		if(mEnv->state()->doc->selection_size() == 1) {
			selection sel = mEnv->state()->doc->selection_at(0);
			if(sel.ent->type() & PLANE) {
				Geom3d::Plane_abstr* sketchPlane = dynamic_cast<Geom3d::Plane_abstr*>(sel.ent);
				start_sketch(sketchPlane, sel.camSt);
				sel.ent->set_hover(false);
			}
		}
	}
}

bool StartSketch_tool::manage_button_press(GdkEventButton* event)
{
	if(mEnv->state()) {
		// If the hovered entity is a plane, start sketch
		Drawable* ent = entity_at_point(glm::vec2(event->x, event->y));
		if(ent && ent->type() & PLANE) {
			Geom3d::Plane_abstr* sketchPlane = dynamic_cast<Geom3d::Plane_abstr*>(ent);
			start_sketch(sketchPlane, mEnv->state()->cam->state());
			ent->set_hover(false);
		}
	}
	return true;
}

void StartSketch_tool::act_on_entity(Drawable* ent)
{
	if(mEnv->state() && ent->type() & PLANE)
		start_sketch(dynamic_cast<Geom3d::Plane_abstr*>(ent), mEnv->state()->cam->state());
}

void StartSketch_tool::start_sketch(Geom3d::Plane_abstr* sketchPlane, CameraState const& camState_)
{
	// mEnv->state()->doc->make_glContext_current();
	mCurrentSketch = new Sketch(sketchPlane);
	mEnv->target()->add_sketch(mCurrentSketch);
	mEnv->state()->doc->push_action(std::shared_ptr<Serial_action>(new Serial_action({
		Action_ptr(new ToggleBaseObject_action(mCurrentSketch, true)),
		Action_ptr(new EnterSketch_action(mCurrentSketch, true)),
		Action_ptr(new QuitPartDesign_action()),
		MoveCamera_action::create_from_facingPlane(sketchPlane, 8.0, camState_, nullptr)
	})));
}