
set(sk_source_files
SkLineCurve.cpp
SkLine.cpp
SkPoint.cpp
SkSelRect.cpp
SkSprite.cpp
SkConstraint.cpp
)

add_library(sk ${sk_source_files})

target_link_libraries(sk
geometry
graphics_utils
)