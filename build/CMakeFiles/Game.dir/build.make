# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/Users/ichrysou/Desktop/sdl_game/sdl_game

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build

# Include any dependencies generated for this target.
include CMakeFiles/Game.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Game.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Game.dir/flags.make

CMakeFiles/Game.dir/main.obj: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/main.obj: CMakeFiles/Game.dir/includes_CXX.rsp
CMakeFiles/Game.dir/main.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Game.dir/main.obj"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/main.obj -c C:/Users/ichrysou/Desktop/sdl_game/sdl_game/main.cpp

CMakeFiles/Game.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/main.i"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/ichrysou/Desktop/sdl_game/sdl_game/main.cpp > CMakeFiles/Game.dir/main.i

CMakeFiles/Game.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/main.s"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/ichrysou/Desktop/sdl_game/sdl_game/main.cpp -o CMakeFiles/Game.dir/main.s

CMakeFiles/Game.dir/game.obj: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/game.obj: CMakeFiles/Game.dir/includes_CXX.rsp
CMakeFiles/Game.dir/game.obj: ../game.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Game.dir/game.obj"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/game.obj -c C:/Users/ichrysou/Desktop/sdl_game/sdl_game/game.cpp

CMakeFiles/Game.dir/game.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/game.i"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/ichrysou/Desktop/sdl_game/sdl_game/game.cpp > CMakeFiles/Game.dir/game.i

CMakeFiles/Game.dir/game.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/game.s"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/ichrysou/Desktop/sdl_game/sdl_game/game.cpp -o CMakeFiles/Game.dir/game.s

CMakeFiles/Game.dir/textureManager.obj: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/textureManager.obj: CMakeFiles/Game.dir/includes_CXX.rsp
CMakeFiles/Game.dir/textureManager.obj: ../textureManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Game.dir/textureManager.obj"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/textureManager.obj -c C:/Users/ichrysou/Desktop/sdl_game/sdl_game/textureManager.cpp

CMakeFiles/Game.dir/textureManager.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/textureManager.i"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/ichrysou/Desktop/sdl_game/sdl_game/textureManager.cpp > CMakeFiles/Game.dir/textureManager.i

CMakeFiles/Game.dir/textureManager.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/textureManager.s"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/ichrysou/Desktop/sdl_game/sdl_game/textureManager.cpp -o CMakeFiles/Game.dir/textureManager.s

CMakeFiles/Game.dir/GameObject.obj: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/GameObject.obj: CMakeFiles/Game.dir/includes_CXX.rsp
CMakeFiles/Game.dir/GameObject.obj: ../GameObject.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Game.dir/GameObject.obj"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/GameObject.obj -c C:/Users/ichrysou/Desktop/sdl_game/sdl_game/GameObject.cpp

CMakeFiles/Game.dir/GameObject.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/GameObject.i"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/ichrysou/Desktop/sdl_game/sdl_game/GameObject.cpp > CMakeFiles/Game.dir/GameObject.i

CMakeFiles/Game.dir/GameObject.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/GameObject.s"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/ichrysou/Desktop/sdl_game/sdl_game/GameObject.cpp -o CMakeFiles/Game.dir/GameObject.s

CMakeFiles/Game.dir/Map.obj: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/Map.obj: CMakeFiles/Game.dir/includes_CXX.rsp
CMakeFiles/Game.dir/Map.obj: ../Map.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Game.dir/Map.obj"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/Map.obj -c C:/Users/ichrysou/Desktop/sdl_game/sdl_game/Map.cpp

CMakeFiles/Game.dir/Map.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/Map.i"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/ichrysou/Desktop/sdl_game/sdl_game/Map.cpp > CMakeFiles/Game.dir/Map.i

CMakeFiles/Game.dir/Map.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/Map.s"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/ichrysou/Desktop/sdl_game/sdl_game/Map.cpp -o CMakeFiles/Game.dir/Map.s

CMakeFiles/Game.dir/Vector2D.obj: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/Vector2D.obj: CMakeFiles/Game.dir/includes_CXX.rsp
CMakeFiles/Game.dir/Vector2D.obj: ../Vector2D.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Game.dir/Vector2D.obj"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/Vector2D.obj -c C:/Users/ichrysou/Desktop/sdl_game/sdl_game/Vector2D.cpp

CMakeFiles/Game.dir/Vector2D.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/Vector2D.i"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/ichrysou/Desktop/sdl_game/sdl_game/Vector2D.cpp > CMakeFiles/Game.dir/Vector2D.i

CMakeFiles/Game.dir/Vector2D.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/Vector2D.s"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/ichrysou/Desktop/sdl_game/sdl_game/Vector2D.cpp -o CMakeFiles/Game.dir/Vector2D.s

CMakeFiles/Game.dir/Collision.obj: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/Collision.obj: CMakeFiles/Game.dir/includes_CXX.rsp
CMakeFiles/Game.dir/Collision.obj: ../Collision.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Game.dir/Collision.obj"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/Collision.obj -c C:/Users/ichrysou/Desktop/sdl_game/sdl_game/Collision.cpp

CMakeFiles/Game.dir/Collision.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/Collision.i"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/ichrysou/Desktop/sdl_game/sdl_game/Collision.cpp > CMakeFiles/Game.dir/Collision.i

CMakeFiles/Game.dir/Collision.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/Collision.s"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/ichrysou/Desktop/sdl_game/sdl_game/Collision.cpp -o CMakeFiles/Game.dir/Collision.s

CMakeFiles/Game.dir/ECS/ECS.obj: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/ECS/ECS.obj: CMakeFiles/Game.dir/includes_CXX.rsp
CMakeFiles/Game.dir/ECS/ECS.obj: ../ECS/ECS.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Game.dir/ECS/ECS.obj"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/ECS/ECS.obj -c C:/Users/ichrysou/Desktop/sdl_game/sdl_game/ECS/ECS.cpp

CMakeFiles/Game.dir/ECS/ECS.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/ECS/ECS.i"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Users/ichrysou/Desktop/sdl_game/sdl_game/ECS/ECS.cpp > CMakeFiles/Game.dir/ECS/ECS.i

CMakeFiles/Game.dir/ECS/ECS.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/ECS/ECS.s"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Users/ichrysou/Desktop/sdl_game/sdl_game/ECS/ECS.cpp -o CMakeFiles/Game.dir/ECS/ECS.s

# Object files for target Game
Game_OBJECTS = \
"CMakeFiles/Game.dir/main.obj" \
"CMakeFiles/Game.dir/game.obj" \
"CMakeFiles/Game.dir/textureManager.obj" \
"CMakeFiles/Game.dir/GameObject.obj" \
"CMakeFiles/Game.dir/Map.obj" \
"CMakeFiles/Game.dir/Vector2D.obj" \
"CMakeFiles/Game.dir/Collision.obj" \
"CMakeFiles/Game.dir/ECS/ECS.obj"

# External object files for target Game
Game_EXTERNAL_OBJECTS =

Game.exe: CMakeFiles/Game.dir/main.obj
Game.exe: CMakeFiles/Game.dir/game.obj
Game.exe: CMakeFiles/Game.dir/textureManager.obj
Game.exe: CMakeFiles/Game.dir/GameObject.obj
Game.exe: CMakeFiles/Game.dir/Map.obj
Game.exe: CMakeFiles/Game.dir/Vector2D.obj
Game.exe: CMakeFiles/Game.dir/Collision.obj
Game.exe: CMakeFiles/Game.dir/ECS/ECS.obj
Game.exe: CMakeFiles/Game.dir/build.make
Game.exe: CMakeFiles/Game.dir/linklibs.rsp
Game.exe: CMakeFiles/Game.dir/objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable Game.exe"
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && "C:/Program Files/CMake/bin/cmake.exe" -E remove -f CMakeFiles/Game.dir/objects.a
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && "C:/Program Files (x86)/mingw-w64/i686-8.1.0-posix-dwarf-rt_v6-rev0/mingw32/bin/ar.exe" cr CMakeFiles/Game.dir/objects.a @CMakeFiles/Game.dir/objects1.rsp
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && g++ -g   -Wl,--whole-archive CMakeFiles/Game.dir/objects.a -Wl,--no-whole-archive  -o Game.exe -Wl,--out-implib,libGame.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/Game.dir/linklibs.rsp

# Rule to build all files generated by this target.
CMakeFiles/Game.dir/build: Game.exe

.PHONY : CMakeFiles/Game.dir/build

CMakeFiles/Game.dir/clean:
	cd C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build && $(CMAKE_COMMAND) -P CMakeFiles/Game.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Game.dir/clean

CMakeFiles/Game.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Users/ichrysou/Desktop/sdl_game/sdl_game C:/Users/ichrysou/Desktop/sdl_game/sdl_game C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build C:/Users/ichrysou/Desktop/sdl_game/sdl_game/build/CMakeFiles/Game.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Game.dir/depend

