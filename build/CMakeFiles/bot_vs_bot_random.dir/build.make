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
CMAKE_SOURCE_DIR = /home/dev/kingchess_remote/kingchess_c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dev/kingchess_remote/kingchess_c/build

# Include any dependencies generated for this target.
include CMakeFiles/bot_vs_bot_random.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/bot_vs_bot_random.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/bot_vs_bot_random.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bot_vs_bot_random.dir/flags.make

CMakeFiles/bot_vs_bot_random.dir/fundamental/neighbor.cpp.o: CMakeFiles/bot_vs_bot_random.dir/flags.make
CMakeFiles/bot_vs_bot_random.dir/fundamental/neighbor.cpp.o: /home/dev/kingchess_remote/kingchess_c/fundamental/neighbor.cpp
CMakeFiles/bot_vs_bot_random.dir/fundamental/neighbor.cpp.o: CMakeFiles/bot_vs_bot_random.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dev/kingchess_remote/kingchess_c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/bot_vs_bot_random.dir/fundamental/neighbor.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bot_vs_bot_random.dir/fundamental/neighbor.cpp.o -MF CMakeFiles/bot_vs_bot_random.dir/fundamental/neighbor.cpp.o.d -o CMakeFiles/bot_vs_bot_random.dir/fundamental/neighbor.cpp.o -c /home/dev/kingchess_remote/kingchess_c/fundamental/neighbor.cpp

CMakeFiles/bot_vs_bot_random.dir/fundamental/neighbor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bot_vs_bot_random.dir/fundamental/neighbor.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dev/kingchess_remote/kingchess_c/fundamental/neighbor.cpp > CMakeFiles/bot_vs_bot_random.dir/fundamental/neighbor.cpp.i

CMakeFiles/bot_vs_bot_random.dir/fundamental/neighbor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bot_vs_bot_random.dir/fundamental/neighbor.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dev/kingchess_remote/kingchess_c/fundamental/neighbor.cpp -o CMakeFiles/bot_vs_bot_random.dir/fundamental/neighbor.cpp.s

CMakeFiles/bot_vs_bot_random.dir/fundamental/bot_vs_bot_random.cpp.o: CMakeFiles/bot_vs_bot_random.dir/flags.make
CMakeFiles/bot_vs_bot_random.dir/fundamental/bot_vs_bot_random.cpp.o: /home/dev/kingchess_remote/kingchess_c/fundamental/bot_vs_bot_random.cpp
CMakeFiles/bot_vs_bot_random.dir/fundamental/bot_vs_bot_random.cpp.o: CMakeFiles/bot_vs_bot_random.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dev/kingchess_remote/kingchess_c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/bot_vs_bot_random.dir/fundamental/bot_vs_bot_random.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bot_vs_bot_random.dir/fundamental/bot_vs_bot_random.cpp.o -MF CMakeFiles/bot_vs_bot_random.dir/fundamental/bot_vs_bot_random.cpp.o.d -o CMakeFiles/bot_vs_bot_random.dir/fundamental/bot_vs_bot_random.cpp.o -c /home/dev/kingchess_remote/kingchess_c/fundamental/bot_vs_bot_random.cpp

CMakeFiles/bot_vs_bot_random.dir/fundamental/bot_vs_bot_random.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bot_vs_bot_random.dir/fundamental/bot_vs_bot_random.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dev/kingchess_remote/kingchess_c/fundamental/bot_vs_bot_random.cpp > CMakeFiles/bot_vs_bot_random.dir/fundamental/bot_vs_bot_random.cpp.i

CMakeFiles/bot_vs_bot_random.dir/fundamental/bot_vs_bot_random.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bot_vs_bot_random.dir/fundamental/bot_vs_bot_random.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dev/kingchess_remote/kingchess_c/fundamental/bot_vs_bot_random.cpp -o CMakeFiles/bot_vs_bot_random.dir/fundamental/bot_vs_bot_random.cpp.s

CMakeFiles/bot_vs_bot_random.dir/fundamental/utils.cpp.o: CMakeFiles/bot_vs_bot_random.dir/flags.make
CMakeFiles/bot_vs_bot_random.dir/fundamental/utils.cpp.o: /home/dev/kingchess_remote/kingchess_c/fundamental/utils.cpp
CMakeFiles/bot_vs_bot_random.dir/fundamental/utils.cpp.o: CMakeFiles/bot_vs_bot_random.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dev/kingchess_remote/kingchess_c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/bot_vs_bot_random.dir/fundamental/utils.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bot_vs_bot_random.dir/fundamental/utils.cpp.o -MF CMakeFiles/bot_vs_bot_random.dir/fundamental/utils.cpp.o.d -o CMakeFiles/bot_vs_bot_random.dir/fundamental/utils.cpp.o -c /home/dev/kingchess_remote/kingchess_c/fundamental/utils.cpp

CMakeFiles/bot_vs_bot_random.dir/fundamental/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bot_vs_bot_random.dir/fundamental/utils.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dev/kingchess_remote/kingchess_c/fundamental/utils.cpp > CMakeFiles/bot_vs_bot_random.dir/fundamental/utils.cpp.i

CMakeFiles/bot_vs_bot_random.dir/fundamental/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bot_vs_bot_random.dir/fundamental/utils.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dev/kingchess_remote/kingchess_c/fundamental/utils.cpp -o CMakeFiles/bot_vs_bot_random.dir/fundamental/utils.cpp.s

CMakeFiles/bot_vs_bot_random.dir/fundamental/board.cpp.o: CMakeFiles/bot_vs_bot_random.dir/flags.make
CMakeFiles/bot_vs_bot_random.dir/fundamental/board.cpp.o: /home/dev/kingchess_remote/kingchess_c/fundamental/board.cpp
CMakeFiles/bot_vs_bot_random.dir/fundamental/board.cpp.o: CMakeFiles/bot_vs_bot_random.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dev/kingchess_remote/kingchess_c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/bot_vs_bot_random.dir/fundamental/board.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bot_vs_bot_random.dir/fundamental/board.cpp.o -MF CMakeFiles/bot_vs_bot_random.dir/fundamental/board.cpp.o.d -o CMakeFiles/bot_vs_bot_random.dir/fundamental/board.cpp.o -c /home/dev/kingchess_remote/kingchess_c/fundamental/board.cpp

CMakeFiles/bot_vs_bot_random.dir/fundamental/board.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bot_vs_bot_random.dir/fundamental/board.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dev/kingchess_remote/kingchess_c/fundamental/board.cpp > CMakeFiles/bot_vs_bot_random.dir/fundamental/board.cpp.i

CMakeFiles/bot_vs_bot_random.dir/fundamental/board.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bot_vs_bot_random.dir/fundamental/board.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dev/kingchess_remote/kingchess_c/fundamental/board.cpp -o CMakeFiles/bot_vs_bot_random.dir/fundamental/board.cpp.s

CMakeFiles/bot_vs_bot_random.dir/fundamental/gamestate.cpp.o: CMakeFiles/bot_vs_bot_random.dir/flags.make
CMakeFiles/bot_vs_bot_random.dir/fundamental/gamestate.cpp.o: /home/dev/kingchess_remote/kingchess_c/fundamental/gamestate.cpp
CMakeFiles/bot_vs_bot_random.dir/fundamental/gamestate.cpp.o: CMakeFiles/bot_vs_bot_random.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dev/kingchess_remote/kingchess_c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/bot_vs_bot_random.dir/fundamental/gamestate.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bot_vs_bot_random.dir/fundamental/gamestate.cpp.o -MF CMakeFiles/bot_vs_bot_random.dir/fundamental/gamestate.cpp.o.d -o CMakeFiles/bot_vs_bot_random.dir/fundamental/gamestate.cpp.o -c /home/dev/kingchess_remote/kingchess_c/fundamental/gamestate.cpp

CMakeFiles/bot_vs_bot_random.dir/fundamental/gamestate.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bot_vs_bot_random.dir/fundamental/gamestate.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dev/kingchess_remote/kingchess_c/fundamental/gamestate.cpp > CMakeFiles/bot_vs_bot_random.dir/fundamental/gamestate.cpp.i

CMakeFiles/bot_vs_bot_random.dir/fundamental/gamestate.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bot_vs_bot_random.dir/fundamental/gamestate.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dev/kingchess_remote/kingchess_c/fundamental/gamestate.cpp -o CMakeFiles/bot_vs_bot_random.dir/fundamental/gamestate.cpp.s

CMakeFiles/bot_vs_bot_random.dir/fundamental/coordinate.cpp.o: CMakeFiles/bot_vs_bot_random.dir/flags.make
CMakeFiles/bot_vs_bot_random.dir/fundamental/coordinate.cpp.o: /home/dev/kingchess_remote/kingchess_c/fundamental/coordinate.cpp
CMakeFiles/bot_vs_bot_random.dir/fundamental/coordinate.cpp.o: CMakeFiles/bot_vs_bot_random.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dev/kingchess_remote/kingchess_c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/bot_vs_bot_random.dir/fundamental/coordinate.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bot_vs_bot_random.dir/fundamental/coordinate.cpp.o -MF CMakeFiles/bot_vs_bot_random.dir/fundamental/coordinate.cpp.o.d -o CMakeFiles/bot_vs_bot_random.dir/fundamental/coordinate.cpp.o -c /home/dev/kingchess_remote/kingchess_c/fundamental/coordinate.cpp

CMakeFiles/bot_vs_bot_random.dir/fundamental/coordinate.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bot_vs_bot_random.dir/fundamental/coordinate.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dev/kingchess_remote/kingchess_c/fundamental/coordinate.cpp > CMakeFiles/bot_vs_bot_random.dir/fundamental/coordinate.cpp.i

CMakeFiles/bot_vs_bot_random.dir/fundamental/coordinate.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bot_vs_bot_random.dir/fundamental/coordinate.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dev/kingchess_remote/kingchess_c/fundamental/coordinate.cpp -o CMakeFiles/bot_vs_bot_random.dir/fundamental/coordinate.cpp.s

CMakeFiles/bot_vs_bot_random.dir/fundamental/agent.cpp.o: CMakeFiles/bot_vs_bot_random.dir/flags.make
CMakeFiles/bot_vs_bot_random.dir/fundamental/agent.cpp.o: /home/dev/kingchess_remote/kingchess_c/fundamental/agent.cpp
CMakeFiles/bot_vs_bot_random.dir/fundamental/agent.cpp.o: CMakeFiles/bot_vs_bot_random.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dev/kingchess_remote/kingchess_c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/bot_vs_bot_random.dir/fundamental/agent.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/bot_vs_bot_random.dir/fundamental/agent.cpp.o -MF CMakeFiles/bot_vs_bot_random.dir/fundamental/agent.cpp.o.d -o CMakeFiles/bot_vs_bot_random.dir/fundamental/agent.cpp.o -c /home/dev/kingchess_remote/kingchess_c/fundamental/agent.cpp

CMakeFiles/bot_vs_bot_random.dir/fundamental/agent.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bot_vs_bot_random.dir/fundamental/agent.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dev/kingchess_remote/kingchess_c/fundamental/agent.cpp > CMakeFiles/bot_vs_bot_random.dir/fundamental/agent.cpp.i

CMakeFiles/bot_vs_bot_random.dir/fundamental/agent.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bot_vs_bot_random.dir/fundamental/agent.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dev/kingchess_remote/kingchess_c/fundamental/agent.cpp -o CMakeFiles/bot_vs_bot_random.dir/fundamental/agent.cpp.s

# Object files for target bot_vs_bot_random
bot_vs_bot_random_OBJECTS = \
"CMakeFiles/bot_vs_bot_random.dir/fundamental/neighbor.cpp.o" \
"CMakeFiles/bot_vs_bot_random.dir/fundamental/bot_vs_bot_random.cpp.o" \
"CMakeFiles/bot_vs_bot_random.dir/fundamental/utils.cpp.o" \
"CMakeFiles/bot_vs_bot_random.dir/fundamental/board.cpp.o" \
"CMakeFiles/bot_vs_bot_random.dir/fundamental/gamestate.cpp.o" \
"CMakeFiles/bot_vs_bot_random.dir/fundamental/coordinate.cpp.o" \
"CMakeFiles/bot_vs_bot_random.dir/fundamental/agent.cpp.o"

# External object files for target bot_vs_bot_random
bot_vs_bot_random_EXTERNAL_OBJECTS =

bot_vs_bot_random: CMakeFiles/bot_vs_bot_random.dir/fundamental/neighbor.cpp.o
bot_vs_bot_random: CMakeFiles/bot_vs_bot_random.dir/fundamental/bot_vs_bot_random.cpp.o
bot_vs_bot_random: CMakeFiles/bot_vs_bot_random.dir/fundamental/utils.cpp.o
bot_vs_bot_random: CMakeFiles/bot_vs_bot_random.dir/fundamental/board.cpp.o
bot_vs_bot_random: CMakeFiles/bot_vs_bot_random.dir/fundamental/gamestate.cpp.o
bot_vs_bot_random: CMakeFiles/bot_vs_bot_random.dir/fundamental/coordinate.cpp.o
bot_vs_bot_random: CMakeFiles/bot_vs_bot_random.dir/fundamental/agent.cpp.o
bot_vs_bot_random: CMakeFiles/bot_vs_bot_random.dir/build.make
bot_vs_bot_random: CMakeFiles/bot_vs_bot_random.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dev/kingchess_remote/kingchess_c/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable bot_vs_bot_random"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bot_vs_bot_random.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bot_vs_bot_random.dir/build: bot_vs_bot_random
.PHONY : CMakeFiles/bot_vs_bot_random.dir/build

CMakeFiles/bot_vs_bot_random.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bot_vs_bot_random.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bot_vs_bot_random.dir/clean

CMakeFiles/bot_vs_bot_random.dir/depend:
	cd /home/dev/kingchess_remote/kingchess_c/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dev/kingchess_remote/kingchess_c /home/dev/kingchess_remote/kingchess_c /home/dev/kingchess_remote/kingchess_c/build /home/dev/kingchess_remote/kingchess_c/build /home/dev/kingchess_remote/kingchess_c/build/CMakeFiles/bot_vs_bot_random.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bot_vs_bot_random.dir/depend

