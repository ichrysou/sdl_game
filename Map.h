#pragma once
#include <string>
#include <vector>
#include <SDL.h>

class Map
{
private:
public:
  Map();
  ~Map();
  //    static void LoadMap(std::string path, int sizeX, int sizeY);

  static bool LoadMapXml(std::string path);
  static void AddTile(int srcX, int srcY, int x, int y, int width, int height, int rotation, SDL_RendererFlip flip, int scale);
  static int WorldWidth();
  static int WorldHeight();
  static const std::vector<SDL_Point> &TorchPositions();

private:
};
