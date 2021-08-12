
add_subdirectory(Geom2d)
add_subdirectory(geometry_3d)

set(geometry_source_files
  geometry_abstract.cpp)

add_library(geometry ${geometry_source_files})

target_link_libraries(geometry
  Geom2d
  geometry_3d
  ConstraintsSolver)