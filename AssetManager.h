#pragma once

#include <map>
#include <string>
#include "SDL.h"
#include "Vector2D.h"
#include "ECS/ECS.h"

class AssetManager
{
private:
  Manager *manager;
  std::map<std::string, SDL_Texture *> textures;

public:
  AssetManager(Manager *man);
  ~AssetManager();
  // game objects
  void CreateProjectile(Vector2D pos, int range, int speed, std::string id);
  // Texture Management
  void AddTexture(std::string id, const char *path);
  SDL_Texture *GetTexture(std::string id);
};