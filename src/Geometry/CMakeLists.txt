

set(Geom_source_files
	2d/Geom2d_abstr.cpp
	2d/Point_abstr.cpp
	2d/Line_abstr.cpp
	2d/Circle_abstr.cpp

	3d/Geom3d_abstr.cpp
	3d/Plane_abstr.cpp
	3d/Point_abstr.cpp)

add_library(Geom ${Geom_source_files})

target_link_libraries(Geom)
