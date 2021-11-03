
#ifndef TOGGLEBASECONSTRAINT_ACTION_HPP_
#define TOGGLEBASECONSTRAINT_ACTION_HPP_

#include <Actions/Action.hpp>

#include <Drawables/Containers/Sketch.hpp>
#include <ConstraintsSolver/Constraint_abstr.hpp>

/*
	@class ToggleConstraint_action enables and disables a constraint and ensure constraint system
	stays valid

	@parent Action
*/
class ToggleConstraint_action : public Action {
private:
	Sketch* mSketch_parent;     // Sketch that contains the constraint system (which contains the constraint)
	Constraint_abstr* mConstraint;  // Constraint to toggle
	bool mToggle_to, mDone;     // State it is in when the Action is created and if it is already in the right state at creation
public:
	/*
		@constructor ToggleConstraint_action

		@param parent_sketch Sketch that ultimatly contains the constraint
		@param constr           Constraint to toggle
		@param toggle_to        State to toggle to on do (True: exists, False: doesn't exist)
	*/
	ToggleConstraint_action(Sketch* parent_sketch, Constraint_abstr* constr, bool toggle_to, bool done_at_creation);

	/*
		@function do_work toggles the constraint to mToggle_to state and 
		solves the system
		
		@param caller	The document calling the function
		@return bool	True, because it can be done in one go
	*/
	virtual bool do_work(Document* caller);
	/*
		@function undo_work toggles the constraint to !mToggle_to state and 
		solves the system
		
		@param caller	The document calling the function
		@return bool	True, because it can be done in one go
	*/
	virtual bool undo_work(Document* caller);
};

#endif