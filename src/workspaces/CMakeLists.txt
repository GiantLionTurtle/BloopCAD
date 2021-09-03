set(workspaces_Utils_source_files
  navigationBar.cpp
  partDesign.cpp
  sketchDesign.cpp
  home.cpp
  workspace.cpp)

add_library(workspaces ${workspaces_Utils_source_files})

target_link_libraries(workspaces
  Utils
	Tools
  )