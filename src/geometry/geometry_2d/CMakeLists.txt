
set(geometry_2d_source_files
  Geom2d_abstr.cpp
  Point_abstr.cpp
  Line_abstr.cpp
  Circle_abstr.cpp)

add_library(geometry_2d ${geometry_2d_source_files})

target_link_libraries(geometry_2d
    constraintsSolver)