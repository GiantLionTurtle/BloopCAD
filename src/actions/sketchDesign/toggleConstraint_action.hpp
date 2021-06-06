
#ifndef TOGGLEBASECONSTRAINT_ACTION_HPP_
#define TOGGLEBASECONSTRAINT_ACTION_HPP_

#include <actions/action.hpp>
#include <constraintsSolver/constraint.hpp>
#include <entities/sketch.hpp>

class toggleConstraint_action : public action {
private:
    sketch_ptr mSketch_parent;
    std::shared_ptr<constraint_abstract> mConstraint;
    bool mToggle_to;
public:
	toggleConstraint_action(sketch_ptr parent_sketch, std::shared_ptr<constraint_abstract> constr, bool toggle_to);

	virtual bool do_work(document* caller);
	virtual bool undo_work(document* caller);
};

#endif