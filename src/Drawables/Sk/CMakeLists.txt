
set(sk_source_files
SkLineCurve.cpp
SkCircleCurve.cpp
SkPoint.cpp
SkSelRect.cpp
SkSprite.cpp
SkConstraint.cpp
)

add_library(sk ${sk_source_files})

target_link_libraries(sk
Geom2d
Geom3d
Graphics_utils
)