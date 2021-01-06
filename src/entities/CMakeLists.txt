add_subdirectory(geometry)
#add_subdirectory(sketchEntities)

set(entities_source_files
  entity.cpp
  plane.cpp
  line.cpp
  point.cpp
  part.cpp
  sketch.cpp
  folder.cpp
  entityView.cpp)

add_library(entities ${entities_source_files})

target_link_libraries(entities
  geometry
  constraintsSolver
	graphics_utils
  )