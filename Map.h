#pragma once
#include <string>

class Map
{
private:
public:
  Map();
  ~Map();
  //    static void LoadMap(std::string path, int sizeX, int sizeY);

  static void LoadMapXml(std::string path);
  static void AddTile(int srcX, int srcY, int x, int y, int width, int height, int rotation, int scale);

private:
};
