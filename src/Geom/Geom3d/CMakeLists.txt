
set(Geom3d_source_files
  Geom3d_abstr.cpp
  Plane_abstr.cpp
  Point_abstr.cpp)

add_library(Geom3d ${Geom3d_source_files})

target_link_libraries(Geom3d
    ConstraintsSolver)