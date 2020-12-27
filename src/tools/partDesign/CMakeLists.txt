set(partDesign_tools_source_files
    startSketch_tool.cpp
  )

add_library(partDesign_tools ${partDesign_tools_source_files})

target_link_libraries(partDesign_tools
  common_tools
  actions
  )