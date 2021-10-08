
#ifndef QUITPARTDESIGN_ACTION_HPP_
#define QUITPARTDESIGN_ACTION_HPP_

#include <Actions/Action.hpp>
#include <Drawables/Containers/Part.hpp>

#include <memory>

/*
	@class QuitPart_action cleans up the Part workspace and Part target before another workspace is activated
	this currently consists of hiding or not the origin of the part

	@parent Action
	@note Might be concatenated with EnterPart_action as the origin visibility logic is duplicated
*/
class QuitPart_action : public Action {
private:
	Part* mTarget;
public:
	QuitPart_action();

	/*
		@function do_work decides if the origin should be seen after the document is 
		no longer in part workspace

		@param caller	The document calling the function
		@return bool	True, because it can be done in one go
	*/
	virtual bool do_work(Document* caller);
	/*
		@function do_work decides if the origin should be seen when the document is 
		in part workspace
		
		@param caller	The document calling the function
		@return bool	True, because it can be done in one go	*/
	virtual bool undo_work(Document* caller);
};

#endif