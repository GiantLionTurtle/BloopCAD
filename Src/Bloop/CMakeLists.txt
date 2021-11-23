set(Bloop_source_files
	Bloop.cpp
)

add_library(Bloop ${Bloop_source_files})

target_link_libraries(Bloop
	Workspaces
)