
set(Geom2d_source_files
  Geom2d_abstr.cpp
  Point_abstr.cpp
  Line_abstr.cpp
  Circle_abstr.cpp)

add_library(Geom2d ${Geom2d_source_files})

target_link_libraries(Geom2d
    constraintsSolver)