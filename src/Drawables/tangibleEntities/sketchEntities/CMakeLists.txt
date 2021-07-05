
set(sketchEntities_source_files
  sketchGeometry.cpp
  sketchPoint.cpp
  sketchLine.cpp
  sketchCircle.cpp
  constraintAnnotation.cpp
  spriteAnnotation.cpp
  sketchConstraint.cpp
  selectionRectangle.cpp
  )

add_library(sketchEntities ${sketchEntities_source_files})

target_link_libraries(sketchEntities
  geometry
  graphics_utils
  tangibleEntities
  )