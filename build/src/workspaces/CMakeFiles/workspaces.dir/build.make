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
include src/workspaces/CMakeFiles/workspaces.dir/depend.make

# Include the progress variables for this target.
include src/workspaces/CMakeFiles/workspaces.dir/progress.make

# Include the compile flags for this target's objects.
include src/workspaces/CMakeFiles/workspaces.dir/flags.make

src/workspaces/CMakeFiles/workspaces.dir/partDesign.cpp.o: src/workspaces/CMakeFiles/workspaces.dir/flags.make
src/workspaces/CMakeFiles/workspaces.dir/partDesign.cpp.o: ../src/workspaces/partDesign.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/theo/Desktop/BloopCAD/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/workspaces/CMakeFiles/workspaces.dir/partDesign.cpp.o"
	cd /home/theo/Desktop/BloopCAD/build/src/workspaces && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/workspaces.dir/partDesign.cpp.o -c /home/theo/Desktop/BloopCAD/src/workspaces/partDesign.cpp

src/workspaces/CMakeFiles/workspaces.dir/partDesign.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/workspaces.dir/partDesign.cpp.i"
	cd /home/theo/Desktop/BloopCAD/build/src/workspaces && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/theo/Desktop/BloopCAD/src/workspaces/partDesign.cpp > CMakeFiles/workspaces.dir/partDesign.cpp.i

src/workspaces/CMakeFiles/workspaces.dir/partDesign.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/workspaces.dir/partDesign.cpp.s"
	cd /home/theo/Desktop/BloopCAD/build/src/workspaces && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/theo/Desktop/BloopCAD/src/workspaces/partDesign.cpp -o CMakeFiles/workspaces.dir/partDesign.cpp.s

src/workspaces/CMakeFiles/workspaces.dir/sketchDesign.cpp.o: src/workspaces/CMakeFiles/workspaces.dir/flags.make
src/workspaces/CMakeFiles/workspaces.dir/sketchDesign.cpp.o: ../src/workspaces/sketchDesign.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/theo/Desktop/BloopCAD/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/workspaces/CMakeFiles/workspaces.dir/sketchDesign.cpp.o"
	cd /home/theo/Desktop/BloopCAD/build/src/workspaces && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/workspaces.dir/sketchDesign.cpp.o -c /home/theo/Desktop/BloopCAD/src/workspaces/sketchDesign.cpp

src/workspaces/CMakeFiles/workspaces.dir/sketchDesign.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/workspaces.dir/sketchDesign.cpp.i"
	cd /home/theo/Desktop/BloopCAD/build/src/workspaces && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/theo/Desktop/BloopCAD/src/workspaces/sketchDesign.cpp > CMakeFiles/workspaces.dir/sketchDesign.cpp.i

src/workspaces/CMakeFiles/workspaces.dir/sketchDesign.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/workspaces.dir/sketchDesign.cpp.s"
	cd /home/theo/Desktop/BloopCAD/build/src/workspaces && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/theo/Desktop/BloopCAD/src/workspaces/sketchDesign.cpp -o CMakeFiles/workspaces.dir/sketchDesign.cpp.s

src/workspaces/CMakeFiles/workspaces.dir/workspace.cpp.o: src/workspaces/CMakeFiles/workspaces.dir/flags.make
src/workspaces/CMakeFiles/workspaces.dir/workspace.cpp.o: ../src/workspaces/workspace.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/theo/Desktop/BloopCAD/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/workspaces/CMakeFiles/workspaces.dir/workspace.cpp.o"
	cd /home/theo/Desktop/BloopCAD/build/src/workspaces && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/workspaces.dir/workspace.cpp.o -c /home/theo/Desktop/BloopCAD/src/workspaces/workspace.cpp

src/workspaces/CMakeFiles/workspaces.dir/workspace.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/workspaces.dir/workspace.cpp.i"
	cd /home/theo/Desktop/BloopCAD/build/src/workspaces && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/theo/Desktop/BloopCAD/src/workspaces/workspace.cpp > CMakeFiles/workspaces.dir/workspace.cpp.i

src/workspaces/CMakeFiles/workspaces.dir/workspace.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/workspaces.dir/workspace.cpp.s"
	cd /home/theo/Desktop/BloopCAD/build/src/workspaces && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/theo/Desktop/BloopCAD/src/workspaces/workspace.cpp -o CMakeFiles/workspaces.dir/workspace.cpp.s

# Object files for target workspaces
workspaces_OBJECTS = \
"CMakeFiles/workspaces.dir/partDesign.cpp.o" \
"CMakeFiles/workspaces.dir/sketchDesign.cpp.o" \
"CMakeFiles/workspaces.dir/workspace.cpp.o"

# External object files for target workspaces
workspaces_EXTERNAL_OBJECTS =

src/workspaces/libworkspaces.a: src/workspaces/CMakeFiles/workspaces.dir/partDesign.cpp.o
src/workspaces/libworkspaces.a: src/workspaces/CMakeFiles/workspaces.dir/sketchDesign.cpp.o
src/workspaces/libworkspaces.a: src/workspaces/CMakeFiles/workspaces.dir/workspace.cpp.o
src/workspaces/libworkspaces.a: src/workspaces/CMakeFiles/workspaces.dir/build.make
src/workspaces/libworkspaces.a: src/workspaces/CMakeFiles/workspaces.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/theo/Desktop/BloopCAD/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libworkspaces.a"
	cd /home/theo/Desktop/BloopCAD/build/src/workspaces && $(CMAKE_COMMAND) -P CMakeFiles/workspaces.dir/cmake_clean_target.cmake
	cd /home/theo/Desktop/BloopCAD/build/src/workspaces && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/workspaces.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/workspaces/CMakeFiles/workspaces.dir/build: src/workspaces/libworkspaces.a

.PHONY : src/workspaces/CMakeFiles/workspaces.dir/build

src/workspaces/CMakeFiles/workspaces.dir/clean:
	cd /home/theo/Desktop/BloopCAD/build/src/workspaces && $(CMAKE_COMMAND) -P CMakeFiles/workspaces.dir/cmake_clean.cmake
.PHONY : src/workspaces/CMakeFiles/workspaces.dir/clean

src/workspaces/CMakeFiles/workspaces.dir/depend:
	cd /home/theo/Desktop/BloopCAD/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/theo/Desktop/BloopCAD /home/theo/Desktop/BloopCAD/src/workspaces /home/theo/Desktop/BloopCAD/build /home/theo/Desktop/BloopCAD/build/src/workspaces /home/theo/Desktop/BloopCAD/build/src/workspaces/CMakeFiles/workspaces.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/workspaces/CMakeFiles/workspaces.dir/depend

