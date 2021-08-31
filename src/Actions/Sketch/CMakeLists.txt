set(Sketch_actions_source_files
  EnterSketch_action.cpp
  QuitSketch_action.cpp
  ToggleConstraint_action.cpp
  ApplySnapshot_action.cpp)

add_library(Sketch_actions ${Sketch_actions_source_files})

target_link_libraries(Sketch_actions
  Common_actions
  )