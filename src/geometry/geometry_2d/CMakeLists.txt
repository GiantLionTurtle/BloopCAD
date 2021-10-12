
set(geometry_2d_source_files
  geometry_2d_abstr.cpp
  point_abstr.cpp
  line_abstr.cpp
  circle_abstr.cpp)

add_library(geometry_2d ${geometry_2d_source_files})

target_link_libraries(geometry_2d
    constraintsSolver)