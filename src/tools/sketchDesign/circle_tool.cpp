
#include "circle_tool.hpp"
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <Drawables/Sketch.hpp>
#include <actions/common/toggleBaseObject_action.hpp>
#include <utils/mathUtils.hpp>

#include <glm/gtx/quaternion.hpp>

circle_tool::circle_tool(sketchDesign* env):
	tool(env),
	mCirclePreview(nullptr)
{
	DEBUG_ASSERT(mEnv, "No valid workspace.");
}

void circle_tool::init()
{
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
	mStarted = false; // Bring flag down
	mCirclePreview = nullptr;
}
void circle_tool::finish()
{
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
	mEnv->target()->clear_toolPreview();
}

bool circle_tool::manage_key_press(GdkEventKey* event)
{
	if(mStarted && event->keyval == GDK_KEY_Escape) {
		mStarted = false;
		mCirclePreview->set_exists(false);
		return false;
	}
	return true;
}

bool circle_tool::manage_mouse_move(GdkEventMotion* event)
{
	if(mStarted) {
		Camera_ptr cam = mEnv->state()->cam; // For ease of writing
		glm::vec2 circle_pos = mCirclePreview->basePlane()->to_planePos(
            mCirclePreview->basePlane()->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));
        mCirclePreview->set_radius(glm::length(circle_pos - mCirclePreview->posCenter()));
	}
	return true;
}
bool circle_tool::manage_button_press(GdkEventButton* event)
{
	// Most of the code of this function will be abstracted eventually because projecting a point 
	// on screen on a point on a plane is pretty basic
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
	
	Sketch* target = mEnv->target();		
	DEBUG_ASSERT(target, "No valid target.");

    // Find where the ray intersectpos_on_plane
    Camera_ptr cam = mEnv->state()->cam; // For ease of writing
    geom_3d::plane_abstr_ptr pl = target->basePlane();
    glm::vec2 circle_pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));

	if(!mStarted) {
		// mEnv->state()->doc->make_glContext_current();
		mCirclePreview = sketchCircle_ptr(new sketchCircle(circle_pos, 0.0f, target->basePlane()));
		mEnv->target()->add_toolPreview(mCirclePreview);
        mStarted = true;
	} else {
		target->add_geometry(mCirclePreview);
		mEnv->state()->doc->push_action(std::make_shared<toggleBaseObject_action>(mCirclePreview, true)); // Doc is passed to activate glContext
        mCirclePreview->set_radius(glm::length(circle_pos - mCirclePreview->posCenter()));
        mStarted = false;
	}
	return true;
}
