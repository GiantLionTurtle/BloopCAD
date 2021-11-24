
// BloopCAD
// Copyright (C) 2020  BloopCorp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef PARALLEL_ACTION_HPP_
#define PARALLEL_ACTION_HPP_

#include <Actions/Action.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

/*
	@class Parallel_action allows to execute multiple actions during the same ticks

	@parent Action
	@note This is not a multithreading framework
*/
class Parallel_action : public Action {
private:
	std::vector<Action_ptr> mActions; // All the actions that will be executed at the "same time" 
    std::vector<bool> mActions_done; // Keep track of which actions are done and undone
public:
	/*
		@constructor Parallel_action

		@param actions All the actions that must be executed "simultaneously"
		@note All actions are assumed to not be done on creation
	*/
	Parallel_action(std::vector<Action_ptr> actions);
	/*
		@constructor Parallel_action creates an empty container that will be filled with
		the add_action function
	*/
	Parallel_action();

	/*
		@function do_work does one 'do_work' tick of every action that is not finished

		@param caller: The document calling the function
		@return False if there is one action that is not done, True if they are all done
	*/
	virtual bool do_work(Document* caller);
	/*
		@function undo_work does one 'undo_work' tick of every action that is not finished

		@param caller: The document calling the function
		@return False if there is one action that is not undone, True if they are all undone
	*/
	virtual bool undo_work(Document* caller);

	/*
		@function add_action adds an action to the list of actions to execute

		@param anAction The Action that must be added to the list
	*/
	void add_action(Action_ptr anAction);

	/*
		@function size

		@return The number of Actions managed by this Action 
	*/
	int size() const { return mActions.size(); }
};

#endif