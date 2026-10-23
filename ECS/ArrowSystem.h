#pragma once
#include "../Collision.h"
#include "ColliderComponent.h"
#include "ProjectileComponent.h"
#include "game.h"

class ArrowSystem {
   private:
    Manager &m_manager;
    Entity *m_pendingThunderOwner = nullptr;
    Uint32 m_thunderReleaseAt = 0;
    Vector2D m_pendingThunderDirection;

   public:
    ArrowSystem(Manager &manager) : m_manager(manager) {}
    void update() {
        // TODO: find a more performant way (creating/destroying vectors on stack
        // doesn't seem great)
        // TODO: THE BEST: implement a filter mechanism like entt that gives you
        // e.g. all collider enemies etc.
        // auto projectiles = m_manager.getEntities<ProjectileComponent>();
        auto players = m_manager.getGroup(Game::groupPlayers);
        for (auto &player : players) {
            auto keyboard = &player->getComponent<KeyboardController>();

            if (keyboard->spacePressedThisFrame) {
                auto &transform = player->getComponent<TransformComponent>();
                auto &animation = player->getComponent<AnimationComponent>();
                auto &sprite = player->getComponent<SpriteComponent>();
                if (sprite.isThunderCasting() || m_pendingThunderOwner) {
                    keyboard->clearTransientInput();
                    continue;
                }
                if (transform.orientation.x != 0.0f) {
                    animation.setTextureOverride("hero-cast");
                    animation.play("thunder-right");
                    sprite.spriteflip = transform.orientation.x < 0
                                            ? SDL_FLIP_HORIZONTAL
                                            : SDL_FLIP_NONE;
                    sprite.setSrcFrame(0, 0, 30, 24);
                } else {
                    animation.setTextureOverride("hero-cast");
                    const bool castingUp = transform.orientation.y < 0;
                    animation.play(castingUp ? "thunder-up" : "thunder-down");
                    sprite.setSrcFrame(0, castingUp ? 48 : 24, 30, 24);
                }
                sprite.startThunderCast(transform.orientation);
                transform.velocity.Zero();
                m_pendingThunderOwner = player;
                m_pendingThunderDirection = transform.orientation;
                m_thunderReleaseAt = SDL_GetTicks() + 210;
            }
            keyboard->clearTransientInput();
        }

        if (m_pendingThunderOwner &&
            SDL_TICKS_PASSED(SDL_GetTicks(), m_thunderReleaseAt)) {
            auto &transform = m_pendingThunderOwner->getComponent<TransformComponent>();
            auto &sprite = m_pendingThunderOwner->getComponent<SpriteComponent>();
            const Vector2D center(
                transform.position.x + transform.width * transform.scale * 0.5f,
                transform.position.y + transform.height * transform.scale * 0.5f);
            const Vector2D staffTip = sprite.getThunderStaffTipWorld(0.5f);
            const Vector2D originOffset(staffTip.x - center.x, staffTip.y - center.y);
            Game::assets->CreateThunderMelee(
                transform, m_pendingThunderDirection, m_pendingThunderDirection.getAngle(),
                originOffset);
            m_pendingThunderOwner = nullptr;
        }

        auto projectiles = m_manager.getGroup(Game::groupProjectiles);
        auto enemies = m_manager.getGroup(Game::groupEnemies);
        auto tiles = m_manager.getGroup(Game::groupColliders);

        for (auto &projectile : projectiles) {
            if (!projectile->hasComponent<ProjectileComponent>()) continue;
            for (auto &tile : tiles) {
                if (tile->hasComponent<ColliderComponent>()) {
                    auto recA = projectile->getComponent<ColliderComponent>().collider;
                    auto recB = tile->getComponent<ColliderComponent>().collider;
                    if (Collision::AABB(recA, recB)) {
                        projectile->destroy();
                    }
                }
            }
            for (auto &enemy : enemies) {
                if (enemy->hasComponent<DeadComponent>() ||
                    !enemy->hasComponent<ColliderComponent>()) continue;
                if (Collision::AABB(projectile->getComponent<ColliderComponent>().collider,
                                    enemy->getComponent<ColliderComponent>().collider)) {
                    enemy->addComponent<DeadComponent>();
                    enemy->getComponent<AnimationComponent>().setActive("die");  // -->animation system
                    enemy->getComponent<TransformComponent>().velocity.Zero();   // -->enemy movement system
                    projectile->destroy();
                    break;
                }
            }
        }
    }
};
