# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ichrysou/projects/sdl_game

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ichrysou/projects/sdl_game/build

# Include any dependencies generated for this target.
include CMakeFiles/Game.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Game.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Game.dir/flags.make

CMakeFiles/Game.dir/main.cpp.o: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ichrysou/projects/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Game.dir/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/main.cpp.o -c /home/ichrysou/projects/sdl_game/main.cpp

CMakeFiles/Game.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ichrysou/projects/sdl_game/main.cpp > CMakeFiles/Game.dir/main.cpp.i

CMakeFiles/Game.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ichrysou/projects/sdl_game/main.cpp -o CMakeFiles/Game.dir/main.cpp.s

CMakeFiles/Game.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/Game.dir/main.cpp.o.requires

CMakeFiles/Game.dir/main.cpp.o.provides: CMakeFiles/Game.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/Game.dir/main.cpp.o.provides

CMakeFiles/Game.dir/main.cpp.o.provides.build: CMakeFiles/Game.dir/main.cpp.o


CMakeFiles/Game.dir/game.cpp.o: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/game.cpp.o: ../game.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ichrysou/projects/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Game.dir/game.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/game.cpp.o -c /home/ichrysou/projects/sdl_game/game.cpp

CMakeFiles/Game.dir/game.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/game.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ichrysou/projects/sdl_game/game.cpp > CMakeFiles/Game.dir/game.cpp.i

CMakeFiles/Game.dir/game.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/game.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ichrysou/projects/sdl_game/game.cpp -o CMakeFiles/Game.dir/game.cpp.s

CMakeFiles/Game.dir/game.cpp.o.requires:

.PHONY : CMakeFiles/Game.dir/game.cpp.o.requires

CMakeFiles/Game.dir/game.cpp.o.provides: CMakeFiles/Game.dir/game.cpp.o.requires
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/game.cpp.o.provides.build
.PHONY : CMakeFiles/Game.dir/game.cpp.o.provides

CMakeFiles/Game.dir/game.cpp.o.provides.build: CMakeFiles/Game.dir/game.cpp.o


CMakeFiles/Game.dir/textureManager.cpp.o: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/textureManager.cpp.o: ../textureManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ichrysou/projects/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Game.dir/textureManager.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/textureManager.cpp.o -c /home/ichrysou/projects/sdl_game/textureManager.cpp

CMakeFiles/Game.dir/textureManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/textureManager.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ichrysou/projects/sdl_game/textureManager.cpp > CMakeFiles/Game.dir/textureManager.cpp.i

CMakeFiles/Game.dir/textureManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/textureManager.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ichrysou/projects/sdl_game/textureManager.cpp -o CMakeFiles/Game.dir/textureManager.cpp.s

CMakeFiles/Game.dir/textureManager.cpp.o.requires:

.PHONY : CMakeFiles/Game.dir/textureManager.cpp.o.requires

CMakeFiles/Game.dir/textureManager.cpp.o.provides: CMakeFiles/Game.dir/textureManager.cpp.o.requires
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/textureManager.cpp.o.provides.build
.PHONY : CMakeFiles/Game.dir/textureManager.cpp.o.provides

CMakeFiles/Game.dir/textureManager.cpp.o.provides.build: CMakeFiles/Game.dir/textureManager.cpp.o


CMakeFiles/Game.dir/GameObject.cpp.o: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/GameObject.cpp.o: ../GameObject.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ichrysou/projects/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Game.dir/GameObject.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/GameObject.cpp.o -c /home/ichrysou/projects/sdl_game/GameObject.cpp

CMakeFiles/Game.dir/GameObject.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/GameObject.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ichrysou/projects/sdl_game/GameObject.cpp > CMakeFiles/Game.dir/GameObject.cpp.i

CMakeFiles/Game.dir/GameObject.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/GameObject.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ichrysou/projects/sdl_game/GameObject.cpp -o CMakeFiles/Game.dir/GameObject.cpp.s

CMakeFiles/Game.dir/GameObject.cpp.o.requires:

.PHONY : CMakeFiles/Game.dir/GameObject.cpp.o.requires

CMakeFiles/Game.dir/GameObject.cpp.o.provides: CMakeFiles/Game.dir/GameObject.cpp.o.requires
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/GameObject.cpp.o.provides.build
.PHONY : CMakeFiles/Game.dir/GameObject.cpp.o.provides

CMakeFiles/Game.dir/GameObject.cpp.o.provides.build: CMakeFiles/Game.dir/GameObject.cpp.o


CMakeFiles/Game.dir/Map.cpp.o: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/Map.cpp.o: ../Map.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ichrysou/projects/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Game.dir/Map.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/Map.cpp.o -c /home/ichrysou/projects/sdl_game/Map.cpp

CMakeFiles/Game.dir/Map.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/Map.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ichrysou/projects/sdl_game/Map.cpp > CMakeFiles/Game.dir/Map.cpp.i

CMakeFiles/Game.dir/Map.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/Map.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ichrysou/projects/sdl_game/Map.cpp -o CMakeFiles/Game.dir/Map.cpp.s

CMakeFiles/Game.dir/Map.cpp.o.requires:

.PHONY : CMakeFiles/Game.dir/Map.cpp.o.requires

CMakeFiles/Game.dir/Map.cpp.o.provides: CMakeFiles/Game.dir/Map.cpp.o.requires
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/Map.cpp.o.provides.build
.PHONY : CMakeFiles/Game.dir/Map.cpp.o.provides

CMakeFiles/Game.dir/Map.cpp.o.provides.build: CMakeFiles/Game.dir/Map.cpp.o


CMakeFiles/Game.dir/Vector2D.cpp.o: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/Vector2D.cpp.o: ../Vector2D.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ichrysou/projects/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Game.dir/Vector2D.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/Vector2D.cpp.o -c /home/ichrysou/projects/sdl_game/Vector2D.cpp

CMakeFiles/Game.dir/Vector2D.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/Vector2D.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ichrysou/projects/sdl_game/Vector2D.cpp > CMakeFiles/Game.dir/Vector2D.cpp.i

CMakeFiles/Game.dir/Vector2D.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/Vector2D.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ichrysou/projects/sdl_game/Vector2D.cpp -o CMakeFiles/Game.dir/Vector2D.cpp.s

CMakeFiles/Game.dir/Vector2D.cpp.o.requires:

.PHONY : CMakeFiles/Game.dir/Vector2D.cpp.o.requires

CMakeFiles/Game.dir/Vector2D.cpp.o.provides: CMakeFiles/Game.dir/Vector2D.cpp.o.requires
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/Vector2D.cpp.o.provides.build
.PHONY : CMakeFiles/Game.dir/Vector2D.cpp.o.provides

CMakeFiles/Game.dir/Vector2D.cpp.o.provides.build: CMakeFiles/Game.dir/Vector2D.cpp.o


# Object files for target Game
Game_OBJECTS = \
"CMakeFiles/Game.dir/main.cpp.o" \
"CMakeFiles/Game.dir/game.cpp.o" \
"CMakeFiles/Game.dir/textureManager.cpp.o" \
"CMakeFiles/Game.dir/GameObject.cpp.o" \
"CMakeFiles/Game.dir/Map.cpp.o" \
"CMakeFiles/Game.dir/Vector2D.cpp.o"

# External object files for target Game
Game_EXTERNAL_OBJECTS =

Game: CMakeFiles/Game.dir/main.cpp.o
Game: CMakeFiles/Game.dir/game.cpp.o
Game: CMakeFiles/Game.dir/textureManager.cpp.o
Game: CMakeFiles/Game.dir/GameObject.cpp.o
Game: CMakeFiles/Game.dir/Map.cpp.o
Game: CMakeFiles/Game.dir/Vector2D.cpp.o
Game: CMakeFiles/Game.dir/build.make
Game: CMakeFiles/Game.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ichrysou/projects/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable Game"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Game.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Game.dir/build: Game

.PHONY : CMakeFiles/Game.dir/build

CMakeFiles/Game.dir/requires: CMakeFiles/Game.dir/main.cpp.o.requires
CMakeFiles/Game.dir/requires: CMakeFiles/Game.dir/game.cpp.o.requires
CMakeFiles/Game.dir/requires: CMakeFiles/Game.dir/textureManager.cpp.o.requires
CMakeFiles/Game.dir/requires: CMakeFiles/Game.dir/GameObject.cpp.o.requires
CMakeFiles/Game.dir/requires: CMakeFiles/Game.dir/Map.cpp.o.requires
CMakeFiles/Game.dir/requires: CMakeFiles/Game.dir/Vector2D.cpp.o.requires

.PHONY : CMakeFiles/Game.dir/requires

CMakeFiles/Game.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Game.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Game.dir/clean

CMakeFiles/Game.dir/depend:
	cd /home/ichrysou/projects/sdl_game/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ichrysou/projects/sdl_game /home/ichrysou/projects/sdl_game /home/ichrysou/projects/sdl_game/build /home/ichrysou/projects/sdl_game/build /home/ichrysou/projects/sdl_game/build/CMakeFiles/Game.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Game.dir/depend
