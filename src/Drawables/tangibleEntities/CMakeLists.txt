
set(tangibleEntities_source_files
  Plane.cpp
  Point.cpp)

add_library(tangibleEntities ${tangibleEntities_source_files})

target_link_libraries(tangibleEntities
  Geom2d
  Geom3d
  Graphics_utils
  )