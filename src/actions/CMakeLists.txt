
add_subdirectory(common)
add_subdirectory(partDesign)
add_subdirectory(sketchDesign)

set(actions_source_files
  action.cpp)

add_library(actions ${actions_source_files})

target_link_libraries(actions
  common_actions
  partDesign_actions
  sketchDesign_actions
  )