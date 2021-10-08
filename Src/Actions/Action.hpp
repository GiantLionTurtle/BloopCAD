
#ifndef ACTION_HPP_
#define ACTION_HPP_

#include <memory>

class Document;
class Action;

using Action_ptr = std::shared_ptr<Action>;

/*
	@class Action: describes an action performed in a Document
	it can be done and undone to the user's wish.

	E.g Enabling an object: it will set it to existant on do and set it to non-existant on undo
*/
class Action {
public:
	Action() {}
	virtual ~Action() {}

	/*
		@function do_work performs the action, it can be summoned as many times as needed
		if the action requires multiple ticks

		@param caller: The document calling the function
		@return bool: If it is done (will return false if it needs more ticks)
	*/
	virtual bool do_work(Document* caller) = 0;
	/*
		@function undo_work performs the invers of the action, it can be summoned as many times as needed
		if the action requires multiple ticks 
		It should bring back the Document to the state prior to the action

		@param caller: The document calling the function
		@return bool: If it is undone (will return false if it needs more ticks)
	*/
	virtual bool undo_work(Document* caller) = 0;
};

#endif