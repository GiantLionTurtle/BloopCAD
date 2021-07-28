
#include "constraint_tool.hpp"

#include <actions/common/parallel_action.hpp>
#include <actions/sketchDesign/toggleConstraint_action.hpp>
#include <actions/sketchDesign/applySnapshot_action.hpp>
#include <document.hpp>

constraint_tool::constraint_tool(sketchDesign* env, int filter):
	tool<sketchDesign>(env),
	mEntA(nullptr),
	mEntB(nullptr),
	mCurrentHover(nullptr),
	mFilter(filter)
{
	DEBUG_ASSERT(mEnv, "No valid workspace.");
}

void constraint_tool::init()
{
	DEBUG_ASSERT(mEnv->state(), "No valid state.");
	mEntA = nullptr;
	mEntB = nullptr;
	mCurrentHover = nullptr;
}
void constraint_tool::finish()
{
	if(mEntA)
		mEntA->set_hover(false);
	if(mEntB)
		mEntB->set_hover(false);
	if(mCurrentHover)
		mCurrentHover->set_hover(false);
}

bool constraint_tool::manage_button_press(GdkEventButton* event)
{
	glm::vec2 screenPos = glm::vec2(event->x, event->y);
	SkDrawable* ent = mEnv->target()->closest_draggable(screenPos, mEnv->state()->cam.get(), mEnv->state()->cam->cast_ray(screenPos), mFilter).ent;
	
	int ent_state = could_add_entity(ent);
	if(ent_state > add_states::COULDNT_ADD) {
		add_entity(ent);
		ent->select();
		if(ent_state == add_states::WOULD_BE_COMPLETE) {
			add_constraint();
			if(mEntA)
				mEntA->unselect();
			if(mEntB)
				mEntB->unselect();
           	mEntA = nullptr;
            mEntB = nullptr;
        }
	}
	return true;
}
bool constraint_tool::manage_mouse_move(GdkEventMotion* event)
{
	glm::vec2 screenPos = glm::vec2(event->x, event->y);
	SkDrawable* ent = mEnv->target()->closest_draggable(screenPos, mEnv->state()->cam.get(), mEnv->state()->cam->cast_ray(screenPos), mFilter).ent;

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

void constraint_tool::add_entity(SkDrawable* ent)
{
	if(!mEntA) {
		mEntA = ent;
	} else {
		mEntB = ent;
	}
}
void constraint_tool::add_constraint()
{
	SkConstraint* constr = nullptr;
	SkDrawable* priority_ent = nullptr;
	
	create_constraint(constr, priority_ent);

	std::map<var_ptr, float> snp = mEnv->target()->snapshot();
	mEnv->target()->add_constraint(constr, priority_ent);
	mEnv->state()->doc->push_action(std::shared_ptr<parallel_action>(new parallel_action({
		std::make_shared<applySnapshot_action>(mEnv->target()->deltaSnapshot(snp), true),
		std::make_shared<toggleConstraint_action>(mEnv->target(), constr, true)
	})));
}

bool constraint_tool::is_point(SkDrawable* drw)
{
	return drw->type() == Drawable_types::POINT;
}
bool constraint_tool::is_line(SkDrawable* drw)
{
	return drw->type() == Drawable_types::AXIS;
}
bool constraint_tool::is_curve(SkDrawable* drw)
{
	return is_line(drw) || drw->type() == Drawable_types::CURVE;
}
bool constraint_tool::is_point_or_curve(SkDrawable* drw)
{
	return is_point(drw) || is_curve(drw);
}