

set(Geom_source_files
	3d/Geom3d_abstr.cpp
	3d/Plane_abstr.cpp
	3d/Point_abstr.cpp)

add_library(Geom ${Geom_source_files})

target_link_libraries(Geom)
