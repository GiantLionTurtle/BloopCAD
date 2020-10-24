set(entities_utils_source_files
  plane_abstract.cpp
  plane.cpp
  entitiesIndexer.cpp
  entity.cpp)

add_library(entities ${entities_utils_source_files})

target_link_libraries(entities
	graphics_utils
  )