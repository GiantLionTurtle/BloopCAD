set(workspaces_utils_source_files
  navigationBar.cpp
  partDesign.cpp
  sketchDesign.cpp
  home.cpp
  workspace.cpp)

add_library(workspaces ${workspaces_utils_source_files})

target_link_libraries(workspaces
  utils
	Tools
  )