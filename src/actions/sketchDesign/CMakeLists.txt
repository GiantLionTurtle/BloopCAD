set(sketchDesign_actions_source_files
  addLine_action.cpp)

add_library(sketchDesign_actions ${sketchDesign_actions_source_files})

target_link_libraries(sketchDesign_actions
  entities
  )