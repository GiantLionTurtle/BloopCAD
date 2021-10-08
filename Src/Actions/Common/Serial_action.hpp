
#ifndef SERIAL_ACTION_HPP_
#define SERIAL_ACTION_HPP_

#include <Actions/Action.hpp>
#include <Drawables/Base/Drawable.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

/*
	@class Serial_action executes actions in order

	@parent Action
*/
class Serial_action : public Action {
private:
	std::vector<Action_ptr> mActions; // Actions that must be performed
	int mCurrentInd; // Current action to execute (do or undo)
public:
	/*
		@constructor Serial_action

		@param actions All the actions that must be executed in order
		@note All actions are assumed to not be done on creation
	*/
	Serial_action(std::vector<Action_ptr> actions);
	/*
		@constructor Serial_action creates an empty container that will be filled with
		the add_action function
	*/
	Serial_action();

	/*
		@function do_work does a tick for the current action, it it is done, the next action will be next
		if not, the action will receive another tick on the next call

		@param caller: The document calling the function
		@return False if all actions are not undone (if the action id < size()) and True if hey are all done
	*/
	virtual bool do_work(Document* caller);
	/*
		@function do_work does a tick for the current action, it it is done, the previous action will be next
		if not, the action will receive another tick on the next call

		@param caller: The document calling the function
		@return False if all actions are not undone (if the action id > 0) and True if hey are all undone
	*/
	virtual bool undo_work(Document* caller);

	/*
		@function add_action adds an action to be executed at the end of the list

		@param anAction The action to be added at the end of the list
	*/
	void add_action(Action_ptr anAction);

	/*
		@function size

		@return The number of serialized actions in the list
	*/
	int size() const { return mActions.size(); }
	/*
		@function ind

		@return The index of the action that is currently executed (done or undone)
	*/
	int ind() const { return mCurrentInd; }
};

#endif