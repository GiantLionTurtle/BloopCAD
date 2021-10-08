
set(Tools_source_files
	Tool.cpp
	
	Navigation3d/Orbit3d_tool.cpp
	Navigation3d/Pan3d_tool.cpp
	Navigation3d/Zoom3d_tool.cpp

	Part/PartDefault_tool.cpp
	Part/StartSketch_tool.cpp

	Sketch/Pan2d_tool.cpp
	Sketch/Zoom2d_tool.cpp
	Sketch/SketchDefault_tool.cpp
	Sketch/Point_tool.cpp
	Sketch/Line_tool.cpp
	Sketch/Circle_tool.cpp
	Sketch/Constraint_tool.cpp
	Sketch/Coincidence_tool.cpp
	Sketch/Perpendicularity_tool.cpp
	Sketch/Verticality_tool.cpp
	Sketch/Horizontality_tool.cpp
)

add_library(Tools ${Tools_source_files})

target_link_libraries(Tools
	Drawables
	Actions
)