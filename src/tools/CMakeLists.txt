
add_subdirectory(common_tools)
add_subdirectory(navigation)
add_subdirectory(partDesign)
add_subdirectory(sketchDesign)

set(tools_source_files
  tool.cpp)

add_library(tools ${tools_source_files})

target_link_libraries(tools
  entities
  actions
  common_tools
  navigation_tools
  partDesign_tools
  sketchDesign_tools
  )