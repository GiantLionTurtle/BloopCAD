
add_subdirectory(geometry_2d)
add_subdirectory(geometry_3d)

set(geometry_source_files
  geometry_abstract.cpp)

add_library(geometry ${geometry_source_files})

target_link_libraries(geometry
  geometry_2d
  geometry_3d
  constraintsSolver)