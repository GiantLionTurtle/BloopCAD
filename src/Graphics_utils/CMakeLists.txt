set(Graphics_utils_source_files
  GLCall.cpp
  Shader.cpp
  ShadersPool.cpp
  VertexBuffer.cpp
  VertexBufferLayout.cpp
  IndexBuffer.cpp
  VertexArray.cpp
  FrameBuffer.cpp
  Camera.cpp
  Texture.cpp
  Image.cpp
  TexturesPool.cpp)

add_library(Graphics_utils ${Graphics_utils_source_files})

target_link_libraries(Graphics_utils
    vendor
    utils)