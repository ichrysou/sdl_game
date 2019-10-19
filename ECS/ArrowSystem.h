#pragma once
#include "../Collision.h"
#include "ColliderComponent.h"
#include "ProjectileComponent.h"
#include "game.h"

class ArrowSystem
{
private:
  Manager &m_manager;
  bool attack_pressed = false;

public:
  ArrowSystem(Manager &manager) : m_manager(manager) {}
  void update()
  {
    // TODO: find a more performant way (creating/destroying vectors on stack
    // doesn't seem great)
    // TODO: THE BEST: implement a filter mechanism like entt that gives you
    // e.g. all collider enemies etc.
    //auto projectiles = m_manager.getEntities<ProjectileComponent>();
    auto players = m_manager.getGroup(Game::groupPlayers);
    for (auto &player : players)
    {
      auto keyboard = &player->getComponent<KeyboardController>();
      auto animation = &player->getComponent<AnimationComponent>();

      if (keyboard->SPACE)
      {
        attack_pressed = true;
        animation->setActive("bow");
      }
      if (attack_pressed)
      {
        if (!keyboard->SPACE)
        {
          std::cout << "atttacckkk released" << std::endl;
          attack_pressed = false;
          auto transform = player->getComponent<TransformComponent>();
          Game::assets->CreateProjectile(transform.position, transform.orientation, 1000, 1, "arrow", transform.orientation.getAngle());
        }
      }
    }

    auto projectiles = m_manager.getGroup(Game::groupProjectiles);
    auto enemies = m_manager.getGroup(Game::groupEnemies);
    auto tiles = m_manager.getGroup(Game::groupColliders);
    // auto colliders = m_manager.getEntities<ColliderComponent>();
    for (auto &projectile : projectiles)
    {
      for (auto &tile : tiles)
      {
        if (tile->hasComponent<ColliderComponent>())
        {
          if (Collision::AABB(
                  projectile->getComponent<ColliderComponent>().collider,
                  tile->getComponent<ColliderComponent>().collider))
          {
            //std::cout << "hit tileeeee" << std::endl;
          }
        }
      }
      for (auto &enemy : enemies)
      {
        if (enemy->hasComponent<ColliderComponent>())
        {
          if (Collision::AABB(
                  projectile->getComponent<ColliderComponent>().collider,
                  enemy->getComponent<ColliderComponent>().collider))
          {
            //std::cout << "hit enemyyyyyyyy" << std::endl;
          }
        }
      }
      /* for (auto &collider : colliders)
        if (Collision::AABB(
                projectile->getComponent<ColliderComponent>().collider,
                collider->getComponent<ColliderComponent>().collider)) {

          if (collider->hasGroup(Game::groupPlayers)) {
            std::cout << "hit player" << std::endl;
          } else if (collider->hasGroup(Game::groupEnemies)) {
            std::cout << "hit enemy" << std::endl;
          } else if (collider->hasGroup(Game::groupMap)) {
            std::cout << "hit tile" << std::endl;
          } else {
            std::cout << "hit sth else" << std::endl;
          }
          // projectile->destroy();
        } */
    }
  }
};