#pragma once

#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "AssetManager.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

class ColliderComponent;
class Game
{
public:
  Game();
  ~Game();

  void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);

  void handleEvents();
  void update();
  void render();
  void clean();
  bool running() { return isRunning; };

  static AssetManager *assets;
  static SDL_Renderer *renderer;
  static SDL_Event event;
  static bool isRunning;
  static SDL_Rect camera;

  enum groupLabels : std::size_t
  {
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders,
    groupProjectiles
  };

private:
  int cntr;
  SDL_Window *window;
};
