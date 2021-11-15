
add_subdirectory(Utils)
add_subdirectory(Graphics_utils)
add_subdirectory(Geometry)
add_subdirectory(Drawables)
add_subdirectory(Actions)
add_subdirectory(Tools)
add_subdirectory(Workspaces)
add_subdirectory(ConstrsSolver)

# add_subdirectory(Tests)

add_executable(BloopCAD
	Bloop.cpp
	Main.cpp)

target_link_libraries(BloopCAD
	vendor
	Utils
	Graphics_utils
	Drawables
	Tools
	Workspaces
	${GTKMM_LIBRARIES}
	${CMAKE_DL_LIBS} epoxy)

