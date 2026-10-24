#pragma once

#include <map>
#include <string>
#include "SDL.h"
#include "Vector2D.h"
#include "ECS/ECS.h"

class TransformComponent;

class AssetManager
{
private:
  Manager *manager;
  std::map<std::string, SDL_Texture *> textures;

public:
  AssetManager(Manager *man);
  ~AssetManager();
  // game objects
  void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id, int angle);
  void CreateBubble(TransformComponent &owner, Vector2D direction, int diameter);
  void CreateThunderMelee(TransformComponent &playerTransform, Vector2D direction, int angle,
                          Vector2D originOffset);
  // Texture Management
  void AddTexture(std::string id, const char *path);
  SDL_Texture *GetTexture(std::string id);

};
