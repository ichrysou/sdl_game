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

void AssetManager::CreateThunderMelee(Vector2D playerCenter, Vector2D direction, int angle) {
    constexpr float frameSize = 64.0f;
    constexpr float scale = 2.0f;
    constexpr float lateralOffset = 10.0f;
    const float perpendicularX = -direction.y;
    const float perpendicularY = direction.x;
    const float spriteCenterX = playerCenter.x + direction.x * frameSize +
                                perpendicularX * lateralOffset;
    const float spriteCenterY = playerCenter.y + direction.y * frameSize +
                                perpendicularY * lateralOffset;

    auto &attack(manager->addEntity());
    attack.addComponent<TransformComponent>(spriteCenterX - frameSize,
                                             spriteCenterY - frameSize,
                                             64, 64, static_cast<int>(scale));
    attack.addComponent<SpriteComponent>("thunder", angle);
    attack.addComponent<AnimationComponent>("thunder");
    attack.getComponent<AnimationComponent>().addAnimation(
        Animation("melee", "thunder", 11, 60, 0, 0, 64, 64, false));
    attack.addComponent<ThunderMeleeComponent>(*manager, playerCenter, direction);
    attack.addGroup(Game::groupProjectiles);
}

void AssetManager::AddTexture(std::string id, const char *path) { textures.emplace(id, TextureManager::LoadTexture(path)); }

SDL_Texture *AssetManager::GetTexture(std::string id) { return textures[id]; }
