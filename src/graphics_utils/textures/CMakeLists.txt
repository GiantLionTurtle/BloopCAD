set(textures_source_files
    texture.cpp
    image.cpp
    atlas.cpp
    text.cpp
  )

add_library(textures ${textures_source_files})

target_link_libraries(textures
    vendor
    freetype
    )

