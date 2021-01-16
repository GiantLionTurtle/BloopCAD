set(sketchDesign_tools_source_files
    sketchDesignDefault_tool.cpp
    point_tool.cpp
    line_tool.cpp
    coincidence_tool.cpp
    perpendicularity_tool.cpp
    verticality_tool.cpp
    horizontality_tool.cpp
  )

add_library(sketchDesign_tools ${sketchDesign_tools_source_files})

target_link_libraries(sketchDesign_tools
  actions
  constraintsSolver
  )