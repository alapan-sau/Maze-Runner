# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.19.6/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.19.6/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/alapan/learn-open-gl/assignment2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/alapan/learn-open-gl/assignment2/build

# Include any dependencies generated for this target.
include CMakeFiles/Hello-World.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Hello-World.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Hello-World.dir/flags.make

CMakeFiles/Hello-World.dir/source/main.cpp.o: CMakeFiles/Hello-World.dir/flags.make
CMakeFiles/Hello-World.dir/source/main.cpp.o: ../source/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/alapan/learn-open-gl/assignment2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Hello-World.dir/source/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Hello-World.dir/source/main.cpp.o -c /Users/alapan/learn-open-gl/assignment2/source/main.cpp

CMakeFiles/Hello-World.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Hello-World.dir/source/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/alapan/learn-open-gl/assignment2/source/main.cpp > CMakeFiles/Hello-World.dir/source/main.cpp.i

CMakeFiles/Hello-World.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Hello-World.dir/source/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/alapan/learn-open-gl/assignment2/source/main.cpp -o CMakeFiles/Hello-World.dir/source/main.cpp.s

CMakeFiles/Hello-World.dir/source/maze.cpp.o: CMakeFiles/Hello-World.dir/flags.make
CMakeFiles/Hello-World.dir/source/maze.cpp.o: ../source/maze.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/alapan/learn-open-gl/assignment2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Hello-World.dir/source/maze.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Hello-World.dir/source/maze.cpp.o -c /Users/alapan/learn-open-gl/assignment2/source/maze.cpp

CMakeFiles/Hello-World.dir/source/maze.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Hello-World.dir/source/maze.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/alapan/learn-open-gl/assignment2/source/maze.cpp > CMakeFiles/Hello-World.dir/source/maze.cpp.i

CMakeFiles/Hello-World.dir/source/maze.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Hello-World.dir/source/maze.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/alapan/learn-open-gl/assignment2/source/maze.cpp -o CMakeFiles/Hello-World.dir/source/maze.cpp.s

# Object files for target Hello-World
Hello__World_OBJECTS = \
"CMakeFiles/Hello-World.dir/source/main.cpp.o" \
"CMakeFiles/Hello-World.dir/source/maze.cpp.o"

# External object files for target Hello-World
Hello__World_EXTERNAL_OBJECTS =

Hello-World: CMakeFiles/Hello-World.dir/source/main.cpp.o
Hello-World: CMakeFiles/Hello-World.dir/source/maze.cpp.o
Hello-World: CMakeFiles/Hello-World.dir/build.make
Hello-World: libraries/glfw/src/libglfw3.a
Hello-World: libglad.a
Hello-World: /usr/local/lib/libGLEW.2.2.0.dylib
Hello-World: CMakeFiles/Hello-World.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/alapan/learn-open-gl/assignment2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Hello-World"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Hello-World.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Hello-World.dir/build: Hello-World

.PHONY : CMakeFiles/Hello-World.dir/build

CMakeFiles/Hello-World.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Hello-World.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Hello-World.dir/clean

CMakeFiles/Hello-World.dir/depend:
	cd /Users/alapan/learn-open-gl/assignment2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/alapan/learn-open-gl/assignment2 /Users/alapan/learn-open-gl/assignment2 /Users/alapan/learn-open-gl/assignment2/build /Users/alapan/learn-open-gl/assignment2/build /Users/alapan/learn-open-gl/assignment2/build/CMakeFiles/Hello-World.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Hello-World.dir/depend
