
set(Drawables_source_files
	Base/Drawable.cpp
	Containers/Part.cpp
	Containers/Sketch.cpp
	Containers/Folder.cpp
	Base/entityView.cpp
	
	Sk/SkLineCurve.cpp
	Sk/SkCircleCurve.cpp
	Sk/SkPoint.cpp
	Sk/SkSelRect.cpp
	Sk/SkSprite.cpp
	Sk/SkConstraint.cpp

	3d/Plane.cpp
	3d/Point.cpp
)

add_library(Drawables ${Drawables_source_files})

target_link_libraries(Drawables
	Geom
	Graphics_utils
	ConstraintsSolver
)