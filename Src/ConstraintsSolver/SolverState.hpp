
#ifndef SOLVERSTATE_HPP_
#define SOLVERSTATE_HPP_

/*
    @struct SolverState is a centralized state/parameter place for constraint solving
*/
struct SolverState {
    enum solveOutput { RUNNING, SUCCESS, FAILURE, INVALID };
	enum algorithm { None = 0, DogLeg = 1, LevenbergMarquardt = 2 };
 
};

#endif