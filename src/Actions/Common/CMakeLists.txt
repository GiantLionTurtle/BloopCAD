

set(Common_actions_source_files
    SwitchWorkspace_action.cpp
    MoveCamera_action.cpp
    ToggleBaseObject_action.cpp
    Serial_action.cpp
    Parallel_action.cpp
  )

add_library(Common_actions ${Common_actions_source_files})

target_link_libraries(Common_actions
  utils
  Drawables
  )