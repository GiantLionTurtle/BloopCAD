
set(sketchEntities_source_files
  sketchPoint.cpp
  sketchLine.cpp
  )

add_library(sketchEntities ${sketchEntities_source_files})

target_link_libraries(sketchEntities
  geometry
  graphics_utils
  tangibleEntities
  )