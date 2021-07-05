add_subdirectory(sketchEntities)

set(tangibleEntities_source_files
  plane.cpp
  point.cpp)

add_library(tangibleEntities ${tangibleEntities_source_files})

target_link_libraries(tangibleEntities
  geometry
  sketchEntities
  graphics_utils
  )