set(Part_actions_source_files
  EnterPart_action.cpp
  QuitPartDesign_action.cpp)

add_library(Part_actions ${Part_actions_source_files})

target_link_libraries(Part_actions
  Common_actions
  )