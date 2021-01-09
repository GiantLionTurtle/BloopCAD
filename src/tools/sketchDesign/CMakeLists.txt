set(sketchDesign_tools_source_files
    point_tool.cpp
    line_tool.cpp
  )

add_library(sketchDesign_tools ${sketchDesign_tools_source_files})

target_link_libraries(sketchDesign_tools
  actions
  )