# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/theo/Desktop/BloopCAD

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/theo/Desktop/BloopCAD/build

# Include any dependencies generated for this target.
include src/tools/CMakeFiles/tools.dir/depend.make

# Include the progress variables for this target.
include src/tools/CMakeFiles/tools.dir/progress.make

# Include the compile flags for this target's objects.
include src/tools/CMakeFiles/tools.dir/flags.make

src/tools/CMakeFiles/tools.dir/tool.cpp.o: src/tools/CMakeFiles/tools.dir/flags.make
src/tools/CMakeFiles/tools.dir/tool.cpp.o: ../src/tools/tool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/theo/Desktop/BloopCAD/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/tools/CMakeFiles/tools.dir/tool.cpp.o"
	cd /home/theo/Desktop/BloopCAD/build/src/tools && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tools.dir/tool.cpp.o -c /home/theo/Desktop/BloopCAD/src/tools/tool.cpp

src/tools/CMakeFiles/tools.dir/tool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tools.dir/tool.cpp.i"
	cd /home/theo/Desktop/BloopCAD/build/src/tools && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/theo/Desktop/BloopCAD/src/tools/tool.cpp > CMakeFiles/tools.dir/tool.cpp.i

src/tools/CMakeFiles/tools.dir/tool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tools.dir/tool.cpp.s"
	cd /home/theo/Desktop/BloopCAD/build/src/tools && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/theo/Desktop/BloopCAD/src/tools/tool.cpp -o CMakeFiles/tools.dir/tool.cpp.s

# Object files for target tools
tools_OBJECTS = \
"CMakeFiles/tools.dir/tool.cpp.o"

# External object files for target tools
tools_EXTERNAL_OBJECTS =

src/tools/libtools.a: src/tools/CMakeFiles/tools.dir/tool.cpp.o
src/tools/libtools.a: src/tools/CMakeFiles/tools.dir/build.make
src/tools/libtools.a: src/tools/CMakeFiles/tools.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/theo/Desktop/BloopCAD/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libtools.a"
	cd /home/theo/Desktop/BloopCAD/build/src/tools && $(CMAKE_COMMAND) -P CMakeFiles/tools.dir/cmake_clean_target.cmake
	cd /home/theo/Desktop/BloopCAD/build/src/tools && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tools.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/tools/CMakeFiles/tools.dir/build: src/tools/libtools.a

.PHONY : src/tools/CMakeFiles/tools.dir/build

src/tools/CMakeFiles/tools.dir/clean:
	cd /home/theo/Desktop/BloopCAD/build/src/tools && $(CMAKE_COMMAND) -P CMakeFiles/tools.dir/cmake_clean.cmake
.PHONY : src/tools/CMakeFiles/tools.dir/clean

src/tools/CMakeFiles/tools.dir/depend:
	cd /home/theo/Desktop/BloopCAD/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/theo/Desktop/BloopCAD /home/theo/Desktop/BloopCAD/src/tools /home/theo/Desktop/BloopCAD/build /home/theo/Desktop/BloopCAD/build/src/tools /home/theo/Desktop/BloopCAD/build/src/tools/CMakeFiles/tools.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/tools/CMakeFiles/tools.dir/depend

