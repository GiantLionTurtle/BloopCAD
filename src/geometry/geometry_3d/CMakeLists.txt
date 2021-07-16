
set(geometry_3d_source_files
  geometry_3d_abstr.cpp
  plane_abstr.cpp
  Point_abstr.cpp)

add_library(geometry_3d ${geometry_3d_source_files})

target_link_libraries(geometry_3d
    constraintsSolver)