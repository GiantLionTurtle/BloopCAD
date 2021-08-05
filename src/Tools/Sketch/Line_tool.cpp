
#include "Line_tool.hpp"
#include <workspaces/workspace.hpp>
#include <document.hpp>
#include <Drawables/Sketch.hpp>
#include <actions/common/toggleBaseObject_action.hpp>
#include <Tools/Sketch/Coincidence_tool.hpp>

#include <glm/gtx/quaternion.hpp>

Line_tool::Line_tool(sketchDesign* env):
	tool(env),
	mEndPos(nullptr),
	mLinePreview(nullptr),
	mLastAdded(nullptr)
{
	DEBUG_ASSERT(mEnv, "No valid workspace.");
}

void Line_tool::init()
{
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
	mLastAdded = nullptr;
	mStarted = false; // Bring flag down
	mEndPos = nullptr;
}
void Line_tool::finish()
{
	mEnv->state()->doc->clear_toolPreview();
}

bool Line_tool::manage_key_press(GdkEventKey* event)
{
	if(mStarted && event->keyval == GDK_KEY_Escape) {
		mStarted = false;
		mLinePreview->set_exists(false);
		mEndPos = nullptr;
		return false;	
	}
	return true;
}

bool Line_tool::manage_mouse_move(GdkEventMotion* event)
{
	if(mStarted) {
		Sketch* target = mEnv->target();
		DEBUG_ASSERT(target, "No valid target.");

		Camera_ptr cam = mEnv->state()->cam; // For ease of writing
		Geom3d::plane_abstr* pl = target->basePlane();
		glm::vec2 line_pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));
		mLinePreview->ptB()->set(line_pos);
	}
	return true;
}
bool Line_tool::manage_button_press(GdkEventButton* event)
{
	// Find where the ray intersectpos_on_plane
	Camera_ptr cam = mEnv->state()->cam; // For ease of writing
	Geom3d::plane_abstr* pl = mEnv->target()->basePlane();
	glm::vec2 line_pos = pl->to_planePos(pl->line_intersection(cam->pos(), cam->cast_ray(glm::vec2(event->x, event->y), false)));

	add_point(line_pos);
	return true;
}

SkPoint* Line_tool::add_point(glm::vec2 pt)
{
	if(!mStarted) {
		mLinePreview = new SkLine(pt, pt, mEnv->target()->basePlane(), false);
		mEnv->state()->doc->set_toolPreview(mLinePreview);
		mStarted = true;
		return mLinePreview->ptA();
	} else {
		mLinePreview->ptB()->set(pt);
		mEnv->target()->add_geometry(mLinePreview);
		mEnv->state()->doc->push_action(std::make_shared<toggleBaseObject_action>(mLinePreview, true));
		mEnv->state()->doc->clear_toolPreview();

		if(mEndPos) {
			LOG_WARNING("Implement constraints");
			mEnv->coincidence()->add_entity(mEndPos);
			mEnv->coincidence()->add_entity(mLinePreview->ptA());
			mEnv->coincidence()->add_constraint();
		}
		mEndPos = mLinePreview->ptB();
		mLastAdded = mLinePreview;
		mLinePreview = new SkLine(pt, pt, mEnv->target()->basePlane(), false);
		mEnv->state()->doc->set_toolPreview(mLinePreview);
		return mEndPos;
	}
}