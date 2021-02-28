
#include "line_tool.hpp"
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <entities/part.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchLine.hpp>
#include <entities/sketch.hpp>
#include <actions/common/enableEntity_action.hpp>
#include <utils/mathUtils.hpp>

#include <glm/gtx/quaternion.hpp>

line_tool::line_tool(sketchDesign* env):
	tool(env),
	mEndPos(nullptr),
	mLinePreview(nullptr),
	mIsMultiline(true)
{
	DEBUG_ASSERT(mEnv, "No valid workspace.");
}

void line_tool::init()
{
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
	if(!mLinePreview) {
		mEnv->state()->doc->make_glContext_current();
		mLinePreview = sketchLine_ptr(new sketchLine(glm::vec2(0.0), glm::vec2(0.0), mEnv->target()->basePlane()));
	}
	mIsMultiline = true;
	started = false; // Bring flag down
	mEndPos = nullptr;
	mLinePreview->set_basePlane(mEnv->target()->basePlane());
	mLinePreview->set_exists(false);
	mEnv->target()->add_toolPreviewGeometry(mLinePreview);
}
void line_tool::finish()
{
	mEnv->target()->clear_toolPreviewGeometries();
}

bool line_tool::manage_key_press(GdkEventKey* event)
{
	if(started) {
		started = false;
		mLinePreview->set_exists(false);
		mEndPos = nullptr;
		return false;
	}
	return true;
}

bool line_tool::manage_mouse_move(GdkEventMotion* event)
{
	if(started) {
		sketch_ptr target = mEnv->target();
		DEBUG_ASSERT(target, "No valid target.");

		camera_ptr cam = mEnv->state()->cam; // For ease of writing
		geom_3d::plane_abstr_ptr pl = target->basePlane();
		glm::vec2 line_pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));
		mLinePreview->B()->set(line_pos);
		target->update();
	}
	return true;
}
bool line_tool::manage_button_press(GdkEventButton* event)
{
	// Most of the code of this function will be abstracted eventually because projecting a point 
	// on screen on a point on a plane is pretty basic	
	sketch_ptr target = mEnv->target();	
	DEBUG_ASSERT(target, "No valid target.");	

	// Find where the ray intersectpos_on_plane
	camera_ptr cam = mEnv->state()->cam; // For ease of writing
	geom_3d::plane_abstr_ptr pl = target->basePlane();
	glm::vec2 line_pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));

	if(!started) {
		mLinePreview->A()->set(line_pos);
		mLinePreview->B()->set(line_pos);
		mLinePreview->set_exists(true);
		started = true;
	} else {
		mEnv->state()->doc->make_glContext_current();
		sketchLine_ptr doneLine = mLinePreview->clone();
		target->add_geometry(doneLine);
		mEnv->state()->doc->push_action(std::make_shared<enableEntity_action>(doneLine)); // Doc is passed to activate glContext
		if(mEndPos) {
			mEnv->target()->add_constraint(pointPoint_distance::make(doneLine->A(), mEndPos, expConst::zero));
		}
		mEndPos = doneLine->B();
		mLinePreview->A()->set(mEndPos->pos());
		// started = false;
	}
	return true;
}
