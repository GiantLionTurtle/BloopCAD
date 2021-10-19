add_executable(ConstraintsSolverTest
    TestConstraintsSolver_main.cpp
	../Bloop.cpp
	)
target_link_libraries(ConstraintsSolverTest
	vendor
	Utils
	Graphics_utils
	Drawables
	Actions
	Tools
	Workspaces
	${GTKMM_LIBRARIES}
	${CMAKE_DL_LIBS} epoxy)