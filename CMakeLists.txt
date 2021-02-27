cmake_minimum_required(VERSION 3.16)
project(BloopCAD)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

SET ( CMAKE_CXX_FLAGS "-std=c++17 -ggdb")
# SET ( CMAKE_CXX_FLAGS "-std=c++17 -O3")

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

include_directories(
    vendor/
	src/
	${GTKMM_INCLUDE_DIRS})

link_directories(
	${GTKMM_LIBRARY_DIRS})

add_subdirectory(vendor)
add_subdirectory(src)
