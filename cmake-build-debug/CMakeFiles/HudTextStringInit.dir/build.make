# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /snap/clion/73/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/73/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ian/CLionProjects/Surface_Graphics

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ian/CLionProjects/Surface_Graphics/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/HudTextStringInit.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/HudTextStringInit.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HudTextStringInit.dir/flags.make

CMakeFiles/HudTextStringInit.dir/Hud/TextStringInit.cpp.o: CMakeFiles/HudTextStringInit.dir/flags.make
CMakeFiles/HudTextStringInit.dir/Hud/TextStringInit.cpp.o: ../Hud/TextStringInit.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ian/CLionProjects/Surface_Graphics/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HudTextStringInit.dir/Hud/TextStringInit.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HudTextStringInit.dir/Hud/TextStringInit.cpp.o -c /home/ian/CLionProjects/Surface_Graphics/Hud/TextStringInit.cpp

CMakeFiles/HudTextStringInit.dir/Hud/TextStringInit.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HudTextStringInit.dir/Hud/TextStringInit.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ian/CLionProjects/Surface_Graphics/Hud/TextStringInit.cpp > CMakeFiles/HudTextStringInit.dir/Hud/TextStringInit.cpp.i

CMakeFiles/HudTextStringInit.dir/Hud/TextStringInit.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HudTextStringInit.dir/Hud/TextStringInit.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ian/CLionProjects/Surface_Graphics/Hud/TextStringInit.cpp -o CMakeFiles/HudTextStringInit.dir/Hud/TextStringInit.cpp.s

# Object files for target HudTextStringInit
HudTextStringInit_OBJECTS = \
"CMakeFiles/HudTextStringInit.dir/Hud/TextStringInit.cpp.o"

# External object files for target HudTextStringInit
HudTextStringInit_EXTERNAL_OBJECTS =

libHudTextStringInit.a: CMakeFiles/HudTextStringInit.dir/Hud/TextStringInit.cpp.o
libHudTextStringInit.a: CMakeFiles/HudTextStringInit.dir/build.make
libHudTextStringInit.a: CMakeFiles/HudTextStringInit.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ian/CLionProjects/Surface_Graphics/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libHudTextStringInit.a"
	$(CMAKE_COMMAND) -P CMakeFiles/HudTextStringInit.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HudTextStringInit.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HudTextStringInit.dir/build: libHudTextStringInit.a

.PHONY : CMakeFiles/HudTextStringInit.dir/build

CMakeFiles/HudTextStringInit.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HudTextStringInit.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HudTextStringInit.dir/clean

CMakeFiles/HudTextStringInit.dir/depend:
	cd /home/ian/CLionProjects/Surface_Graphics/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ian/CLionProjects/Surface_Graphics /home/ian/CLionProjects/Surface_Graphics /home/ian/CLionProjects/Surface_Graphics/cmake-build-debug /home/ian/CLionProjects/Surface_Graphics/cmake-build-debug /home/ian/CLionProjects/Surface_Graphics/cmake-build-debug/CMakeFiles/HudTextStringInit.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HudTextStringInit.dir/depend
