
set(Actions_source_files
	
	# Actions that act within the Sketch workspace
	Sketch/EnterSketch_action.cpp
	Sketch/ToggleConstraint_action.cpp
	Sketch/ApplySnapshot_action.cpp
	
	# Actions that act within the Part workspace
	Part/EnterPart_action.cpp
	Part/QuitPart_action.cpp

	# Actions that do not act within a specific workspace
	Common/SwitchWorkspace_action.cpp
	Common/MoveCamera_action.cpp
	Common/ToggleBaseObject_action.cpp
	Common/Serial_action.cpp
	Common/Parallel_action.cpp
)

add_library(Actions ${Actions_source_files})

target_link_libraries(Actions
	Utils
)