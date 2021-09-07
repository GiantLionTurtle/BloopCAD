
set(Actions_source_files
	Sketch/EnterSketch_action.cpp
	Sketch/QuitSketch_action.cpp
	Sketch/ToggleConstraint_action.cpp
	Sketch/ApplySnapshot_action.cpp
	
	Part/EnterPart_action.cpp
	Part/QuitPartDesign_action.cpp

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