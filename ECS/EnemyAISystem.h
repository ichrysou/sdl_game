#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#include "Collision.h"
#include "Components.h"
#include "ECS.h"
#include "game.h"

// Decides what the enemies do.
//
// An enemy that sees the hero goes for it and keeps coming. The system also
// takes care of how the enemy looks while it does that: it faces the hero and
// walks or stands still depending on whether it actually moved.
class EnemyAISystem
{
private:
    Manager &m_manager;

    bool canSeePlayer(const Entity &enemy, const Entity &hero,
                      const std::vector<Entity *> &walls) const;

public:
    EnemyAISystem(Manager &manager) : m_manager(manager) {}
    void update();
};

inline bool EnemyAISystem::canSeePlayer(const Entity &enemy, const Entity &hero,
                                         const std::vector<Entity *> &walls) const
{
    constexpr float sightRange = 240.0f;
    const auto &enemyCollider = enemy.getComponent<ColliderComponent>().collider;
    const auto &heroCollider = hero.getComponent<ColliderComponent>().collider;
    const float startX = enemyCollider.x + enemyCollider.w * 0.5f;
    const float startY = enemyCollider.y + enemyCollider.h * 0.5f;
    const float endX = heroCollider.x + heroCollider.w * 0.5f;
    const float endY = heroCollider.y + heroCollider.h * 0.5f;
    const float dx = endX - startX;
    const float dy = endY - startY;
    const float distanceSquared = dx * dx + dy * dy;
    if (distanceSquared > sightRange * sightRange)
        return false;

    const int steps = std::max(1, static_cast<int>(std::sqrt(distanceSquared) / 8.0f));
    for (int step = 1; step < steps; ++step)
    {
        const float fraction = static_cast<float>(step) / steps;
        const int x = static_cast<int>(startX + dx * fraction);
        const int y = static_cast<int>(startY + dy * fraction);
        for (const auto *wall : walls)
        {
            if (!wall->hasComponent<ColliderComponent>())
                continue;
            if (Collision::AABB(SDL_Rect{x, y, 1, 1},
                                wall->getComponent<ColliderComponent>().collider))
                return false;
        }
    }
    return true;
}

inline void EnemyAISystem::update()
{
    auto &walls = m_manager.getGroup(Game::groupColliders);
    auto &players = m_manager.getGroup(Game::groupPlayers);
    if (players.empty())
        return;
    Entity &hero = *players.front();
    const auto &heroCollider = hero.getComponent<ColliderComponent>().collider;

    for (auto *enemy : m_manager.getGroup(Game::groupEnemies))
    {
        if (enemy->hasComponent<DeadComponent>())
            continue;
        if (!enemy->hasComponent<EnemyAwarenessComponent>() ||
            !enemy->hasComponent<PathfindingComponent>())
            continue;

        auto &awareness = enemy->getComponent<EnemyAwarenessComponent>();
        auto &path = enemy->getComponent<PathfindingComponent>();
        auto &transform = enemy->getComponent<TransformComponent>();

        if (!awareness.alerted && canSeePlayer(*enemy, hero, walls))
            awareness.alerted = true;

        if (!awareness.alerted)
        {
            path.hasTarget = false;
            if (enemy->hasComponent<AnimationComponent>())
                enemy->getComponent<AnimationComponent>().setActive("idle");
            continue;
        }

        // The hero is the goal of the chase.
        path.target = Vector2D(heroCollider.x + heroCollider.w * 0.5f,
                               heroCollider.y + heroCollider.h * 0.5f);
        path.hasTarget = true;

        if (enemy->hasComponent<SpriteComponent>())
        {
            auto &sprite = enemy->getComponent<SpriteComponent>();
            sprite.spriteflip =
                path.target.x < transform.position.x ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        }
        if (enemy->hasComponent<AnimationComponent>())
        {
            // Walk only when the enemy actually moved: one that is pressed
            // against a wall or the hero stands still.
            const float moved = transform.lastMove.x * transform.lastMove.x +
                                transform.lastMove.y * transform.lastMove.y;
            enemy->getComponent<AnimationComponent>().setActive(moved > 0.01f ? "walk" : "idle");
        }
    }
}
