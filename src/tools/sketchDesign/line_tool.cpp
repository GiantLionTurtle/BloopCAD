
#include "line_tool.hpp"
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <entities/tangibleEntities/sketchEntities/sketchLine.hpp>
#include <entities/sketch.hpp>
#include <actions/common/enableEntity_action.hpp>
#include <utils/mathUtils.hpp>

#include <glm/gtx/quaternion.hpp>

line_tool::line_tool(sketchDesign* env):
	tool(env),
	mEndPos(nullptr),
	mLinePreview(nullptr),
	mLastAdded(nullptr)
{
	DEBUG_ASSERT(mEnv, "No valid workspace.");
}

void line_tool::init()
{
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
	mLastAdded = nullptr;
	mStarted = false; // Bring flag down
	mEndPos = nullptr;
}
void line_tool::finish()
{
	mEnv->target()->clear_toolPreview();
}

bool line_tool::manage_key_press(GdkEventKey* event)
{
	if(mStarted && event->keyval == GDK_KEY_Escape) {
		mStarted = false;
		mLinePreview->set_exists(false);
		mEndPos = nullptr;
		return false;
	}
	return true;
}

bool line_tool::manage_mouse_move(GdkEventMotion* event)
{
	if(mStarted) {
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
	// Find where the ray intersectpos_on_plane
	camera_ptr cam = mEnv->state()->cam; // For ease of writing
	geom_3d::plane_abstr_ptr pl = mEnv->target()->basePlane();
	glm::vec2 line_pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));

	add_point(line_pos);
	return true;
}

sketchPoint_ptr line_tool::add_point(glm::vec2 pt)
{
	if(!mStarted) {
		mLinePreview = std::make_shared<sketchLine>(pt, pt, mEnv->target()->basePlane());
		mEnv->target()->add_toolPreview(mLinePreview);
		mStarted = true;
		return mLinePreview->A();
	} else {
		mLinePreview->B()->set(pt);
		mEnv->target()->add_geometry(mLinePreview);
		mEnv->state()->doc->push_action(std::make_shared<enableEntity_action>(mLinePreview));
		mEnv->target()->clear_toolPreview();

		if(mEndPos) {
			mEnv->target()->add_constraint(pointPoint_distance::make_coincident(mLinePreview->A(), mEndPos));
		}
		mEndPos = mLinePreview->B();
		mLastAdded = mLinePreview;
		mLinePreview = std::make_shared<sketchLine>(pt, pt, mEnv->target()->basePlane());
		mEnv->target()->add_toolPreview(mLinePreview);
		return mEndPos;
	}
}
