# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /snap/clion/235/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /snap/clion/235/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/admin1/mount_admin/MIPT/CAOS/ejudge/contest2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/admin1/mount_admin/MIPT/CAOS/ejudge/contest2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/contest2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/contest2.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/contest2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/contest2.dir/flags.make

CMakeFiles/contest2.dir/4.c.o: CMakeFiles/contest2.dir/flags.make
CMakeFiles/contest2.dir/4.c.o: /home/admin1/mount_admin/MIPT/CAOS/ejudge/contest2/4.c
CMakeFiles/contest2.dir/4.c.o: CMakeFiles/contest2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/admin1/mount_admin/MIPT/CAOS/ejudge/contest2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/contest2.dir/4.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/contest2.dir/4.c.o -MF CMakeFiles/contest2.dir/4.c.o.d -o CMakeFiles/contest2.dir/4.c.o -c /home/admin1/mount_admin/MIPT/CAOS/ejudge/contest2/4.c

CMakeFiles/contest2.dir/4.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/contest2.dir/4.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/admin1/mount_admin/MIPT/CAOS/ejudge/contest2/4.c > CMakeFiles/contest2.dir/4.c.i

CMakeFiles/contest2.dir/4.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/contest2.dir/4.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/admin1/mount_admin/MIPT/CAOS/ejudge/contest2/4.c -o CMakeFiles/contest2.dir/4.c.s

# Object files for target contest2
contest2_OBJECTS = \
"CMakeFiles/contest2.dir/4.c.o"

# External object files for target contest2
contest2_EXTERNAL_OBJECTS =

contest2: CMakeFiles/contest2.dir/4.c.o
contest2: CMakeFiles/contest2.dir/build.make
contest2: CMakeFiles/contest2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/admin1/mount_admin/MIPT/CAOS/ejudge/contest2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable contest2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/contest2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/contest2.dir/build: contest2
.PHONY : CMakeFiles/contest2.dir/build

CMakeFiles/contest2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/contest2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/contest2.dir/clean

CMakeFiles/contest2.dir/depend:
	cd /home/admin1/mount_admin/MIPT/CAOS/ejudge/contest2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/admin1/mount_admin/MIPT/CAOS/ejudge/contest2 /home/admin1/mount_admin/MIPT/CAOS/ejudge/contest2 /home/admin1/mount_admin/MIPT/CAOS/ejudge/contest2/cmake-build-debug /home/admin1/mount_admin/MIPT/CAOS/ejudge/contest2/cmake-build-debug /home/admin1/mount_admin/MIPT/CAOS/ejudge/contest2/cmake-build-debug/CMakeFiles/contest2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/contest2.dir/depend

