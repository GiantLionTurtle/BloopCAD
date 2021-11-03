
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

#include "ApplySnapshot_action.hpp"

#include <Drawables/Containers/Sketch.hpp>

ApplySnapshot_action::ApplySnapshot_action(Sketch* sk, std::vector<ParamDualState> deltas, bool done_at_creation):
	mDeltas(deltas),
	mDone(done_at_creation),
	mSketch(sk)
{

}

bool ApplySnapshot_action::do_work(Document* caller)
{
	if(mDone) // Will skip the first do_work, but will not impeed the others after an undo_work
		return true;
	// mSketch->apply_deltaSnapshot(mDeltas, true); // Apply first set of values
	return true;
}
bool ApplySnapshot_action::undo_work(Document* caller)
{
	mDone = false; // This will never be true again
	// mSketch->apply_deltaSnapshot(mDeltas, false); // Apply second set of values
	return true;
}
