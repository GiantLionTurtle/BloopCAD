
add_subdirectory(utils)
add_subdirectory(graphics_utils)
add_subdirectory(entities)
add_subdirectory(actions)
add_subdirectory(tools)
add_subdirectory(workspaces)

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