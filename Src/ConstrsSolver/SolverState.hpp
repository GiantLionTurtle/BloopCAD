
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

#ifndef SOLVERSTATE_HPP_
#define SOLVERSTATE_HPP_

/*
    @struct SolverState is a centralized state/parameter place for constraint solving
*/
struct SolverState {
    enum solveOutput { RUNNING, SUCCESS, FAILURE, INVALID };
	enum algorithm { None = 0, DogLeg = 1, LevenbergMarquardt = 2 };
    enum graphState { WELL_CONSTRAINED, UNDER_CONSTRAINED, OVER_CONSTRAINED};
};

#endif