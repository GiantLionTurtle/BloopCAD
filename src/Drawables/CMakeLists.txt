add_subdirectory(tangibleEntities)
add_subdirectory(sk)

set(Drawables_source_files
  Drawable.cpp
  Tangible.cpp
  Part.cpp
  Sketch.cpp
  Folder.cpp
  entityView.cpp)

add_library(Drawables ${Drawables_source_files})

target_link_libraries(Drawables
  tangibleEntities
  sk
  constraintsSolver
  )