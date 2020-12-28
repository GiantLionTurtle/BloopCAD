

set(common_actions_source_files
    switchWorkspace_action.cpp
  )

add_library(common_actions ${common_actions_source_files})

target_link_libraries(common_actions
  )