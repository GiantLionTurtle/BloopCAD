
add_subdirectory(utils)
add_subdirectory(navigation)
add_subdirectory(partDesign)

set(tools_source_files
  tool.cpp)

add_library(tools ${tools_source_files})

target_link_libraries(tools
  entities
  actions
  utils_tools
  navigation_tools
  partDesign_tools
  )