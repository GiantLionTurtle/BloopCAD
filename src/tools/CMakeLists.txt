
add_subdirectory(navigation)

set(tools_source_files
  tool.cpp)

add_library(tools ${tools_source_files})

target_link_libraries(tools
  navigation
	entities
  )