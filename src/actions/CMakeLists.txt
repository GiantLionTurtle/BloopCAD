
add_subdirectory(partDesign)

set(actions_source_files
  switchWorkspace_action.cpp)

add_library(actions ${actions_source_files})

target_link_libraries(actions
  partDesign_actions
  )