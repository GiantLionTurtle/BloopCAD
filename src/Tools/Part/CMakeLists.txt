set(Part_tools_source_files
    StartSketch_tool.cpp
    PartDefault_tool.cpp
  )

add_library(Part_tools ${Part_tools_source_files})

target_link_libraries(Part_tools
  Tools
  )