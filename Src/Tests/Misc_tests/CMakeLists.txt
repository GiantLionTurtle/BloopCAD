add_executable(Misc_tests
    Misc_tests.cpp
)
target_link_libraries(Misc_tests
	vendor
	Utils
	Graphics_utils
	Drawables
	Actions
	Tools
	Workspaces
	Bloop
	${GTKMM_LIBRARIES}
	${CMAKE_DL_LIBS} epoxy
)
