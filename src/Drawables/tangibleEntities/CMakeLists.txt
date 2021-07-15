# add_subdirectory(sketchEntities)

set(tangibleEntities_source_files
  Plane.cpp
  Point.cpp)

add_library(tangibleEntities ${tangibleEntities_source_files})

target_link_libraries(tangibleEntities
  geometry
  graphics_utils
  )