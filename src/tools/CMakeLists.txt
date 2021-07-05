
add_subdirectory(common)
add_subdirectory(navigation)
add_subdirectory(partDesign)
add_subdirectory(sketchDesign)

set(tools_source_files
  tool.cpp)

add_library(tools ${tools_source_files})

target_link_libraries(tools
  Drawables
  actions
  common_tools
  navigation_tools
  partDesign_tools
  sketchDesign_tools
  )