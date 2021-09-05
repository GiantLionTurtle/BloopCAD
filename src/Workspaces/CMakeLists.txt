set(Workspaces_Utils_source_files
  NavigationBar.cpp
  Part_ws.cpp
  Sketch_ws.cpp
  Home_ws.cpp
  Workspace_abstr.cpp
  EventsManager.cpp
  )

add_library(Workspaces ${Workspaces_Utils_source_files})

target_link_libraries(Workspaces
  Utils
	Tools
  )