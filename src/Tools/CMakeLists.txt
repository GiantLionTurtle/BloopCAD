
add_subdirectory(Navigation3d)
add_subdirectory(Part)
add_subdirectory(Sketch)

set(Tools_source_files
  Tool.cpp)

add_library(Tools ${Tools_source_files})

target_link_libraries(Tools
  Drawables
  actions
  Navigation3d_tools
  Part_tools
  Sketch_tools
  )