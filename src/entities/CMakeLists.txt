set(entities_utils_source_files
  plane_abstract.cpp
  entity.cpp
  plane.cpp
  line_abstract.cpp
  line.cpp
  point.cpp
  part.cpp
  sketch.cpp)

add_library(entities ${entities_utils_source_files})

target_link_libraries(entities
	graphics_utils
  )