add_executable(ConstrsSolverTest
    TestConstrsSolver_main.cpp
	../Bloop.cpp
	)
target_link_libraries(ConstrsSolverTest
	vendor
	Utils
	Graphics_utils
	Drawables
	Actions
	Tools
	Workspaces
	${GTKMM_LIBRARIES}
	${CMAKE_DL_LIBS} epoxy)