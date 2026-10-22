#include "AssetManager.h"
#include "ECS/Components.h"
#include "textureManager.h"

AssetManager::AssetManager(Manager *man) : manager(man) {}

AssetManager::~AssetManager() {}
void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id, int angle) {
    auto &projectile(manager->addEntity());
    projectile.addComponent<TransformComponent>(pos.x, pos.y, 16, 32, 1);
    // TODO: add this to Vector2D as getAngle()
    projectile.addComponent<SpriteComponent>(id, angle);
    projectile.addComponent<ProjectileComponent>(range, speed, vel);
    projectile.addComponent<ColliderComponent>("projectile", pos.x, pos.y, 16, 32);
    projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::CreateThunderMelee(TransformComponent &playerTransform, Vector2D direction, int angle) {
    auto &attack(manager->addEntity());
    // scale 1 matches the (now scale-1) player; was scale 2 before the shrink
    attack.addComponent<TransformComponent>(playerTransform.position.x,
                                             playerTransform.position.y, 64, 64, 1);
    attack.addComponent<ThunderMeleeComponent>(*manager, playerTransform, direction);
    attack.addComponent<SpriteComponent>("thunder", angle);
    attack.addComponent<AnimationComponent>("thunder");
    attack.getComponent<AnimationComponent>().addAnimation(
        Animation("melee", "thunder", 11, 32, 0, 0, 64, 64, false));
    attack.addGroup(Game::groupProjectiles);
}

void AssetManager::AddTexture(std::string id, const char *path) { textures.emplace(id, TextureManager::LoadTexture(path)); }

SDL_Texture *AssetManager::GetTexture(std::string id) { return textures[id]; }
