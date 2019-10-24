#include "AssetManager.h"
#include "ECS/Components.h"
#include "textureManager.h"

AssetManager::AssetManager(Manager *man) : manager(man) {}

AssetManager::~AssetManager() {}
void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id, int angle) {
    auto &projectile(manager->addEntity());
    projectile.addComponent<TransformComponent>(pos.x, pos.y, 16, 32, 1);
    // TODO: add this to Vector2D as getAngle()
    projectile.addComponent<SpriteComponent>(id, true, angle);
    projectile.addComponent<ProjectileComponent>(range, speed, vel);
    projectile.addComponent<ColliderComponent>("projectile", pos.x, pos.y, 16, 32);
    projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::AddTexture(std::string id, const char *path) { textures.emplace(id, TextureManager::LoadTexture(path)); }

SDL_Texture *AssetManager::GetTexture(std::string id) { return textures[id]; }
