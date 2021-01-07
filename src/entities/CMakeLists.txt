add_subdirectory(geometry)
add_subdirectory(tangibleEntities)

set(entities_source_files
  entity.cpp
  part.cpp
  sketch.cpp
  folder.cpp
  entityView.cpp)

add_library(entities ${entities_source_files})

target_link_libraries(entities
  tangibleEntities
  constraintsSolver
  )