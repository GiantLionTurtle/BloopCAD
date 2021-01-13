
set(geometry_source_files
  point_abstract.cpp
  line_abstract.cpp
  plane_abstract.cpp)

add_library(geometry ${geometry_source_files})

target_link_libraries(geometry
  constraintsSolver)