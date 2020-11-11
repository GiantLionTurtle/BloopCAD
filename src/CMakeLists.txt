
add_subdirectory(graphics_utils)
add_subdirectory(entities)
add_subdirectory(tools)
add_subdirectory(workspaces)

add_executable(BloopCAD
	document.cpp
	navigationBar.cpp
	bloop.cpp
	main.cpp)

target_link_libraries(BloopCAD
	vendor
	graphics_utils
	entities
	tools
	workspaces
	${GTKMM_LIBRARIES}
	${CMAKE_DL_LIBS} epoxy)