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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xuanxuan/code/mprpc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xuanxuan/code/mprpc/build

# Include any dependencies generated for this target.
include example/callee/CMakeFiles/provider.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include example/callee/CMakeFiles/provider.dir/compiler_depend.make

# Include the progress variables for this target.
include example/callee/CMakeFiles/provider.dir/progress.make

# Include the compile flags for this target's objects.
include example/callee/CMakeFiles/provider.dir/flags.make

example/callee/CMakeFiles/provider.dir/friendservice.cc.o: example/callee/CMakeFiles/provider.dir/flags.make
example/callee/CMakeFiles/provider.dir/friendservice.cc.o: /home/xuanxuan/code/mprpc/example/callee/friendservice.cc
example/callee/CMakeFiles/provider.dir/friendservice.cc.o: example/callee/CMakeFiles/provider.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xuanxuan/code/mprpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object example/callee/CMakeFiles/provider.dir/friendservice.cc.o"
	cd /home/xuanxuan/code/mprpc/build/example/callee && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT example/callee/CMakeFiles/provider.dir/friendservice.cc.o -MF CMakeFiles/provider.dir/friendservice.cc.o.d -o CMakeFiles/provider.dir/friendservice.cc.o -c /home/xuanxuan/code/mprpc/example/callee/friendservice.cc

example/callee/CMakeFiles/provider.dir/friendservice.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/provider.dir/friendservice.cc.i"
	cd /home/xuanxuan/code/mprpc/build/example/callee && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xuanxuan/code/mprpc/example/callee/friendservice.cc > CMakeFiles/provider.dir/friendservice.cc.i

example/callee/CMakeFiles/provider.dir/friendservice.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/provider.dir/friendservice.cc.s"
	cd /home/xuanxuan/code/mprpc/build/example/callee && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xuanxuan/code/mprpc/example/callee/friendservice.cc -o CMakeFiles/provider.dir/friendservice.cc.s

example/callee/CMakeFiles/provider.dir/__/friend.pb.cc.o: example/callee/CMakeFiles/provider.dir/flags.make
example/callee/CMakeFiles/provider.dir/__/friend.pb.cc.o: /home/xuanxuan/code/mprpc/example/friend.pb.cc
example/callee/CMakeFiles/provider.dir/__/friend.pb.cc.o: example/callee/CMakeFiles/provider.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xuanxuan/code/mprpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object example/callee/CMakeFiles/provider.dir/__/friend.pb.cc.o"
	cd /home/xuanxuan/code/mprpc/build/example/callee && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT example/callee/CMakeFiles/provider.dir/__/friend.pb.cc.o -MF CMakeFiles/provider.dir/__/friend.pb.cc.o.d -o CMakeFiles/provider.dir/__/friend.pb.cc.o -c /home/xuanxuan/code/mprpc/example/friend.pb.cc

example/callee/CMakeFiles/provider.dir/__/friend.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/provider.dir/__/friend.pb.cc.i"
	cd /home/xuanxuan/code/mprpc/build/example/callee && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xuanxuan/code/mprpc/example/friend.pb.cc > CMakeFiles/provider.dir/__/friend.pb.cc.i

example/callee/CMakeFiles/provider.dir/__/friend.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/provider.dir/__/friend.pb.cc.s"
	cd /home/xuanxuan/code/mprpc/build/example/callee && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xuanxuan/code/mprpc/example/friend.pb.cc -o CMakeFiles/provider.dir/__/friend.pb.cc.s

# Object files for target provider
provider_OBJECTS = \
"CMakeFiles/provider.dir/friendservice.cc.o" \
"CMakeFiles/provider.dir/__/friend.pb.cc.o"

# External object files for target provider
provider_EXTERNAL_OBJECTS =

/home/xuanxuan/code/mprpc/bin/provider: example/callee/CMakeFiles/provider.dir/friendservice.cc.o
/home/xuanxuan/code/mprpc/bin/provider: example/callee/CMakeFiles/provider.dir/__/friend.pb.cc.o
/home/xuanxuan/code/mprpc/bin/provider: example/callee/CMakeFiles/provider.dir/build.make
/home/xuanxuan/code/mprpc/bin/provider: /home/xuanxuan/code/mprpc/lib/libmprpc.so
/home/xuanxuan/code/mprpc/bin/provider: example/callee/CMakeFiles/provider.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xuanxuan/code/mprpc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable /home/xuanxuan/code/mprpc/bin/provider"
	cd /home/xuanxuan/code/mprpc/build/example/callee && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/provider.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/callee/CMakeFiles/provider.dir/build: /home/xuanxuan/code/mprpc/bin/provider
.PHONY : example/callee/CMakeFiles/provider.dir/build

example/callee/CMakeFiles/provider.dir/clean:
	cd /home/xuanxuan/code/mprpc/build/example/callee && $(CMAKE_COMMAND) -P CMakeFiles/provider.dir/cmake_clean.cmake
.PHONY : example/callee/CMakeFiles/provider.dir/clean

example/callee/CMakeFiles/provider.dir/depend:
	cd /home/xuanxuan/code/mprpc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xuanxuan/code/mprpc /home/xuanxuan/code/mprpc/example/callee /home/xuanxuan/code/mprpc/build /home/xuanxuan/code/mprpc/build/example/callee /home/xuanxuan/code/mprpc/build/example/callee/CMakeFiles/provider.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/callee/CMakeFiles/provider.dir/depend

