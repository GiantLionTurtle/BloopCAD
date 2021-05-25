set(sketchDesign_actions_source_files
  enterSketchDesign_action.cpp
  quitSketchDesign_action.cpp
  assignPosSnapshots_action.cpp)

add_library(sketchDesign_actions ${sketchDesign_actions_source_files})

target_link_libraries(sketchDesign_actions
  common_actions
  )