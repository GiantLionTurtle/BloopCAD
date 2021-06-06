
#include "constraint_tool.hpp"

#include <actions/sketchDesign/toggleConstraint_action.hpp>
#include <actions/sketchDesign/assignPosSnapshots_action.hpp>
#include <actions/common/parallel_action.hpp>
#include <document.hpp>

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
	sketchEntity_ptr ent = mEnv->target()->entity_at_point(mEnv->state()->cam, glm::vec2(event->x, event->y));
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
	sketchEntity_ptr ent = mEnv->target()->entity_at_point(mEnv->state()->cam, glm::vec2(event->x, event->y));

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
void constraint_tool::add_constraint()
{
	std::shared_ptr<constraint_abstract> constr = nullptr;
	sketchEntity_ptr priority_ent = nullptr;
	std::vector<entityPosSnapshot_ptr> geom_snapshots(0);

	add_constraint_impl(constr, priority_ent);

	RECORD_SNAPSHOT_DELTAS_IN(mEnv->target(), mEnv->target()->add_constraint(constr, priority_ent), geom_snapshots);
	mEnv->state()->doc->push_action(std::shared_ptr<parallel_action>(new parallel_action({
		std::make_shared<assignPosSnapshots_action>(geom_snapshots),
		std::make_shared<toggleConstraint_action>(mEnv->target(), constr, true)
	})));
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