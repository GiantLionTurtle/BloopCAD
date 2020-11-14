set(partDesign_actions_source_files
  createSketch_action.cpp)

add_library(partDesign_actions ${partDesign_actions_source_files})

target_link_libraries(partDesign_actions
  entities
  )