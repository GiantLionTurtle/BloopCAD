
#ifndef APPLYSNAPSHOT_ACTION_HPP_
#define APPLYSNAPSHOT_ACTION_HPP_

#include <Actions/Action.hpp>
#include <ConstraintsSolver/Expression.hpp>

class Sketch;

/*
	@class ApplySnapshot_action: Sets a group of var_ptr to
	one of two specified values

	@from
*/
class ApplySnapshot_action : public Action {
private:
	std::vector<VarDualState> mDeltas; // Group of variables with their two possible values
	bool mDone; // If the action is done or undone (useful if the variables are already at the done
				// state when the action is created)
	Sketch* mSketch; // The sketch that will be updated after setting the variables
public:
	/*
		@constructor AppluSnapshot_action

		@param sk 				The sketch that contains the variable(but not forced to ) and will be updated after set
		@param deltas			List of all involved var_ptr with their two states
		@param done_at_creation	If the var_ptr are already set to the "done" state
	*/
	ApplySnapshot_action(Sketch* sk, std::vector<VarDualState> deltas, bool done_at_creation);

	/*
		@function do_work sets the var_ptr to their first state

		@param caller	The document calling the function
		@return bool	True, because it can be done in one go
	*/
	virtual bool do_work(Document* caller);
	/*
		@function do_work sets the var_ptr to their second state

		@param caller	The document calling the function
		@return bool	True, because it can be done in one go
	*/
	virtual bool undo_work(Document* caller);
};

#endif