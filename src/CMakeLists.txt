
add_subdirectory(utils)
add_subdirectory(graphics_utils)
add_subdirectory(entities)
add_subdirectory(actions)
add_subdirectory(tools)
add_subdirectory(workspaces)
add_subdirectory(constraintsSolver)

add_executable(BloopCAD
	document.cpp
	bloop.cpp
	main.cpp)

target_link_libraries(BloopCAD
	vendor
	utils
	graphics_utils
	entities
	actions
	tools
	workspaces
	${GTKMM_LIBRARIES}
	${CMAKE_DL_LIBS} epoxy)

# ******************* tests ************************

add_executable(constraintsSolverTest
	testConstraintsSolver_main.cpp
	)
target_link_libraries(constraintsSolverTest
	vendor
	utils
	entities
	constraintsSolver
	${GTKMM_LIBRARIES}
	${CMAKE_DL_LIBS} epoxy)