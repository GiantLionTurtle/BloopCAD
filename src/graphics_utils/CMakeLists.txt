set(graphics_utils_source_files
  GLCall.cpp
  shader.cpp
  vertexBuffer.cpp
  vertexBufferLayout.cpp
  indexBuffer.cpp
  vertexArray.cpp
  frameBuffer.cpp
  camera.cpp)

add_subdirectory(textures)

add_library(graphics_utils ${graphics_utils_source_files})

target_link_libraries(graphics_utils
    utils
    textures)