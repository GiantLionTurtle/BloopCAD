
#ifndef SOLVERSTATE_HPP_
#define SOLVERSTATE_HPP_

struct SolverState {
    enum solveOutput { RUNNING, SUCCESS, FAILURE, INVALID };
	enum algorithm { DogLeg, LevenbergMarquardt };
 
};

#endif