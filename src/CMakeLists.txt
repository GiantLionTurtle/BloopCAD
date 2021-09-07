
add_subdirectory(Utils)
add_subdirectory(Graphics_utils)
add_subdirectory(Geometry)
add_subdirectory(Drawables)
add_subdirectory(Actions)
add_subdirectory(Tools)
add_subdirectory(Workspaces)
add_subdirectory(ConstraintsSolver)

add_executable(BloopCAD
	document.cpp
	bloop.cpp
	main.cpp)

target_link_libraries(BloopCAD
	vendor
	Utils
	Graphics_utils
	Drawables
	Tools
	Workspaces
	${GTKMM_LIBRARIES}
	${CMAKE_DL_LIBS} epoxy)

# ******************* tests ************************

# add_executable(ConstraintsSolverTest
# 	testConstraintsSolver_main.cpp
# 	document.cpp
# 	EventsManager.cpp
# 	testFramework_EventsManager.cpp
# 	bloop.cpp
# 	)
# target_link_libraries(ConstraintsSolverTest
# 	vendor
# 	Utils
# 	Graphics_utils
# 	Drawables
# 	actions
# 	tools
# 	Workspaces
# 	${GTKMM_LIBRARIES}
# 	${CMAKE_DL_LIBS} epoxy)