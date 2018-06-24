project(Game)
add_executable(Game main.cpp game.cpp textureManager.cpp GameObject.cpp Map.cpp)
target_link_libraries(Game SDL2 SDL2_image)
