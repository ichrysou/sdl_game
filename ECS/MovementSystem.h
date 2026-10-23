#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#include "Collision.h"
#include "Components.h"
#include "ECS.h"
#include "game.h"

// Moves the entities through the world.
//
// Integrating the position and resolving the collisions happen together here,
// so a blocked move slides along the wall instead of stopping dead. That is
// what keeps an agent at full speed while it rounds a corner.
class MovementSystem
{
private:
    Manager &m_manager;

public:
    MovementSystem(Manager &manager) : m_manager(manager) {}
    void update();
};

inline void MovementSystem::update()
{
    auto &walls = m_manager.getGroup(Game::groupColliders);
    auto &players = m_manager.getGroup(Game::groupPlayers);
    auto &enemies = m_manager.getGroup(Game::groupEnemies);

    const auto hitsWalls = [&walls](const SDL_Rect &rect) {
        for (const auto *wall : walls)
        {
            if (!wall->hasComponent<ColliderComponent>())
                continue;
            if (Collision::AABB(rect, wall->getComponent<ColliderComponent>().collider))
                return true;
        }
        return false;
    };
    const auto hitsPlayers = [&players](const SDL_Rect &rect) {
        for (const auto *player : players)
        {
            if (!player->hasComponent<ColliderComponent>())
                continue;
            if (Collision::AABB(rect, player->getComponent<ColliderComponent>().collider))
                return true;
        }
        return false;
    };
    const auto hitsLivingEnemies = [&enemies](const SDL_Rect &rect) {
        for (const auto *enemy : enemies)
        {
            if (enemy->hasComponent<DeadComponent>() || !enemy->hasComponent<ColliderComponent>())
                continue;
            if (Collision::AABB(rect, enemy->getComponent<ColliderComponent>().collider))
                return true;
        }
        return false;
    };

    // 1. Integrate the velocity and resolve it against the world.
    for (auto &entity : m_manager.getEntities<TransformComponent>())
    {
        auto &transform = entity->getComponent<TransformComponent>();
        const float startX = transform.position.x;
        const float startY = transform.position.y;
        const float deltaX = transform.velocity.x * transform.speed;
        const float deltaY = transform.velocity.y * transform.speed;

        const bool collides = entity->hasComponent<ColliderComponent>() &&
                              entity->hasComponent<MovementComponent>();
        if (!collides)
        {
            transform.position.x = startX + deltaX;
            transform.position.y = startY + deltaY;
        }
        else
        {
            const auto &collider = entity->getComponent<ColliderComponent>().collider;
            const bool isPlayer = entity->hasGroup(Game::groupPlayers);
            const bool slide = entity->getComponent<MovementComponent>().slideAlongWalls;

            const auto blocked = [&](float x, float y) {
                const SDL_Rect candidate{static_cast<int>(x), static_cast<int>(y),
                                         collider.w, collider.h};
                if (hitsWalls(candidate))
                    return true;
                // A character is in the way of the other one: the hero cannot
                // walk through an enemy and an enemy cannot walk through the
                // hero.
                return isPlayer ? hitsLivingEnemies(candidate) : hitsPlayers(candidate);
            };

            float newX = startX + deltaX;
            float newY = startY + deltaY;
            if (blocked(newX, newY))
            {
                if (!slide)
                {
                    newX = startX;
                    newY = startY;
                }
                else if (blocked(startX + deltaX, startY))
                {
                    // The horizontal part is blocked as well, so only the
                    // vertical one is left. Every attempt starts from the
                    // position before the move, so the agent ends up sliding
                    // along the wall instead of stopping in front of it.
                    if (blocked(startX, startY + deltaY))
                    {
                        newX = startX;
                        newY = startY;
                    }
                    else
                    {
                        newX = startX;
                        newY = startY + deltaY;
                    }
                }
                else
                {
                    newY = startY;
                }
            }
            transform.position.x = newX;
            transform.position.y = newY;
        }

        if (entity->hasComponent<ColliderComponent>())
            entity->getComponent<ColliderComponent>().update();

        transform.lastMove.x = transform.position.x - startX;
        transform.lastMove.y = transform.position.y - startY;
    }

    // 2. Push overlapping enemies apart so they do not stack up in a doorway.
    std::vector<std::pair<Entity *, std::pair<float, float>>> beforeSeparation;
    beforeSeparation.reserve(enemies.size());
    for (auto *enemy : enemies)
    {
        if (enemy->hasComponent<DeadComponent>() || !enemy->hasComponent<MovementComponent>())
            continue;
        if (!enemy->getComponent<MovementComponent>().separateFromAgents)
            continue;
        const auto &position = enemy->getComponent<TransformComponent>().position;
        beforeSeparation.emplace_back(enemy, std::make_pair(position.x, position.y));
    }

    for (std::size_t i = 0; i < enemies.size(); ++i)
    {
        auto *first = enemies[i];
        if (first->hasComponent<DeadComponent>() || !first->hasComponent<MovementComponent>())
            continue;
        for (std::size_t j = i + 1; j < enemies.size(); ++j)
        {
            auto *second = enemies[j];
            if (second->hasComponent<DeadComponent>() || !second->hasComponent<MovementComponent>())
                continue;

            auto &transformA = first->getComponent<TransformComponent>();
            auto &transformB = second->getComponent<TransformComponent>();
            auto &colliderA = first->getComponent<ColliderComponent>();
            auto &colliderB = second->getComponent<ColliderComponent>();
            if (!Collision::AABB(colliderA, colliderB))
                continue;

            const int overlapX = std::min(colliderA.collider.x + colliderA.collider.w,
                                          colliderB.collider.x + colliderB.collider.w) -
                                 std::max(colliderA.collider.x, colliderB.collider.x);
            const int overlapY = std::min(colliderA.collider.y + colliderA.collider.h,
                                          colliderB.collider.y + colliderB.collider.h) -
                                 std::max(colliderA.collider.y, colliderB.collider.y);
            if (overlapX <= 0 || overlapY <= 0)
                continue;

            if (overlapX < overlapY)
            {
                const float direction = transformA.position.x < transformB.position.x ? -1.0f : 1.0f;
                transformA.position.x += direction * overlapX * 0.5f;
                transformB.position.x -= direction * overlapX * 0.5f;
            } else
            {
                const float direction = transformA.position.y < transformB.position.y ? -1.0f : 1.0f;
                transformA.position.y += direction * overlapY * 0.5f;
                transformB.position.y -= direction * overlapY * 0.5f;
            }
            colliderA.update();
            colliderB.update();
        }
    }

    // 3. A separation that pushes an enemy into a wall is undone.
    for (auto &entry : beforeSeparation)
    {
        Entity *enemy = entry.first;
        auto &transform = enemy->getComponent<TransformComponent>();
        auto &collider = enemy->getComponent<ColliderComponent>();
        const SDL_Rect candidate{static_cast<int>(transform.position.x),
                                 static_cast<int>(transform.position.y),
                                 collider.collider.w, collider.collider.h};
        if (hitsWalls(candidate))
        {
            transform.position.x = entry.second.first;
            transform.position.y = entry.second.second;
            collider.update();
        }
    }

    // 4. Contact with the hero flashes the hero, as it always did.
    if (!players.empty())
    {
        auto *hero = players.front();
        if (hero->hasComponent<SpriteComponent>() &&
            hero->hasComponent<ColliderComponent>())
        {
            const auto &heroCollider = hero->getComponent<ColliderComponent>().collider;
            for (const auto *enemy : enemies)
            {
                if (enemy->hasComponent<DeadComponent>() ||
                    !enemy->hasComponent<ColliderComponent>())
                    continue;
                if (Collision::AABB(heroCollider, enemy->getComponent<ColliderComponent>().collider))
                {
                    hero->getComponent<SpriteComponent>().startFlicker();
                    break;
                }
            }
        }
    }
}
