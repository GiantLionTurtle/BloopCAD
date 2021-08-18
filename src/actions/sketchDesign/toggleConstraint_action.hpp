
#ifndef TOGGLEBASECONSTRAINT_ACTION_HPP_
#define TOGGLEBASECONSTRAINT_ACTION_HPP_

#include <actions/action.hpp>

#include <Drawables/Sketch.hpp>
#include <Drawables/Sk/SkConstraint.hpp>

class toggleConstraint_action : public action {
private:
    Sketch* mSketch_parent;
    SkConstraint* mConstraint;
    bool mToggle_to, mDone;
public:
	toggleConstraint_action(Sketch* parent_sketch, SkConstraint* constr, bool toggle_to, bool done_at_creation);

	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);
};

#endif