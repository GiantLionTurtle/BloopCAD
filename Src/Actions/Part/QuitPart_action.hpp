
// BloopCAD
// Copyright (C) 2020-2021 BloopCorp

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

#ifndef QUITPARTDESIGN_ACTION_HPP_
#define QUITPARTDESIGN_ACTION_HPP_

#include <Actions/Action.hpp>

#include <memory>

class Part;

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