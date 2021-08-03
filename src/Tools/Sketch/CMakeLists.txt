set(Sketch_tools_source_files
    Pan2d_tool.cpp
    Zoom2d_tool.cpp
    SketchDefault_tool.cpp
    Point_tool.cpp
    Line_tool.cpp
    Circle_tool.cpp
    Constraint_tool.cpp
    Coincidence_tool.cpp
    Perpendicularity_tool.cpp
    Verticality_tool.cpp
    Horizontality_tool.cpp
  )

add_library(Sketch_tools ${Sketch_tools_source_files})

target_link_libraries(Sketch_tools
  Tools
  actions
  constraintsSolver
  )