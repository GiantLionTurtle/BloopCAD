
#ifndef TOGGLEBASECONSTRAINT_ACTION_HPP_
#define TOGGLEBASECONSTRAINT_ACTION_HPP_

#include <actions/action.hpp>
#include <Drawables/tangibleEntities/sketchEntities/sketchConstraint.hpp>
#include <Drawables/Sketch.hpp>

class toggleConstraint_action : public action {
private:
    Sketch_ptr mSketch_parent;
    std::shared_ptr<constraint_entity> mConstraint;
    bool mToggle_to;
public:
	toggleConstraint_action(Sketch_ptr parent_sketch, std::shared_ptr<constraint_entity> constr, bool toggle_to);

	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);
};

#endif