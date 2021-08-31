
#ifndef ACTION_HPP_
#define ACTION_HPP_

#include <memory>

class document;
class Action;

using Action_ptr = std::shared_ptr<Action>;

/*
	@class action describes an action performed in a document
	it describes the back and forth between an action and it's opposite
*/
class Action {
public:
	Action() {}
	virtual ~Action() {}

	/*
		@function do_work performs the action, it can be summoned as many times as needed
	*/
	virtual bool do_work(document* caller) = 0;
	/*
		@function undo_work performs the invers of the action, it can be summoned as many times as needed
		it should also bring back the document to the state prior to the action
	*/
	virtual bool undo_work(document* caller) = 0;
};

#endif