

set(common_actions_source_files
    switchWorkspace_action.cpp
    moveCamera_action.cpp
    addEntity_action.cpp
    serial_action.cpp
  )

add_library(common_actions ${common_actions_source_files})

target_link_libraries(common_actions
  utils
  entities
  )