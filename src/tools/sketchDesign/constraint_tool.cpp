
#include "constraint_tool.hpp"

constraint_tool::constraint_tool(sketchDesign* env):
	tool<sketchDesign>(env),
	mEntA(nullptr),
	mEntB(nullptr),
	mCurrentHover(nullptr)
{
	DEBUG_ASSERT(mEnv, "No valid workspace.");
}

void constraint_tool::init()
{
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
}

bool constraint_tool::manage_button_press(GdkEventButton* event)
{	
	sketchEntity_ptr ent = mEnv->target()->geometry_at_point(mEnv->state()->cam, glm::vec2(event->x, event->y));
	int ent_state = could_add_entity(ent);
	if(ent_state > add_states::COULDNT_ADD) {
		add_entity(ent);
		if(ent_state == add_states::WOULD_BE_COMPLETE) {
			add_constraint();
           	mEntA = nullptr;
            mEntB = nullptr;
        }
	}
	return true;
}
bool constraint_tool::manage_mouse_move(GdkEventMotion* event)
{
	sketchEntity_ptr ent = mEnv->target()->geometry_at_point(mEnv->state()->cam, glm::vec2(event->x, event->y));

	if(ent != mCurrentHover) {
		if(mCurrentHover) {
			mCurrentHover->set_hover(false);
		}
		if(could_add_entity(ent)) {
			ent->set_hover(true);
		}
		mCurrentHover = ent;
	}
	return true;
}

void constraint_tool::add_entity(sketchEntity_ptr ent)
{
	if(!mEntA) {
		mEntA = ent;
	} else {
		mEntB = ent;
	}
}

bool constraint_tool::is_point(sketchEntity_ptr ent)
{
	return ent->type() == sketchEntity::POINT;
}
bool constraint_tool::is_line(sketchEntity_ptr ent)
{
	return ent->type() == sketchEntity::LINE;
}
bool constraint_tool::is_curve(sketchEntity_ptr ent)
{
	return is_line(ent) || ent->type() == sketchEntity::CIRCLE;
}
bool constraint_tool::is_point_or_curve(sketchEntity_ptr ent)
{
	return is_point(ent) || is_curve(ent);
}