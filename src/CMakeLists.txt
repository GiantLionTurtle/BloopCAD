
add_subdirectory(utils)
add_subdirectory(graphics_utils)
add_subdirectory(geometry)
add_subdirectory(Drawables)
add_subdirectory(actions)
add_subdirectory(Tools)
add_subdirectory(workspaces)
add_subdirectory(constraintsSolver)

add_executable(BloopCAD
	document.cpp
	eventsManager.cpp
	bloop.cpp
	main.cpp)

target_link_libraries(BloopCAD
	vendor
	utils
	graphics_utils
	Drawables
	actions
	Tools
	workspaces
	${GTKMM_LIBRARIES}
	${CMAKE_DL_LIBS} epoxy)

# ******************* tests ************************

# add_executable(constraintsSolverTest
# 	testConstraintsSolver_main.cpp
# 	document.cpp
# 	eventsManager.cpp
# 	testFramework_eventsManager.cpp
# 	bloop.cpp
# 	)
# target_link_libraries(constraintsSolverTest
# 	vendor
# 	utils
# 	graphics_utils
# 	Drawables
# 	actions
# 	tools
# 	workspaces
# 	${GTKMM_LIBRARIES}
# 	${CMAKE_DL_LIBS} epoxy)