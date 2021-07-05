add_subdirectory(tangibleEntities)

set(Drawables_source_files
  Drawable.cpp
  part.cpp
  sketch.cpp
  folder.cpp
  entityView.cpp)

add_library(Drawables ${Drawables_source_files})

target_link_libraries(Drawables
  tangibleEntities
  constraintsSolver
  )