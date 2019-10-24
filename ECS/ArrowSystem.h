#pragma once
#include "../Collision.h"
#include "ColliderComponent.h"
#include "ProjectileComponent.h"
#include "game.h"

class ArrowSystem {
   private:
    Manager &m_manager;
    bool attack_pressed = false;

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
            auto animation = &player->getComponent<AnimationComponent>();

            if (keyboard->SPACE) {
                attack_pressed = true;
                animation->setActive("bow");
            }
            if (attack_pressed) {
                if (!keyboard->SPACE) {

                    attack_pressed = false;
                    auto transform = player->getComponent<TransformComponent>();
                    std::cout << "atttacckkk released x" << transform.position.x << " y " << transform.position.y << std::endl;
                    Game::assets->CreateProjectile(transform.position, transform.orientation, 1000, 1, "arrow",
                                                   transform.orientation.getAngle());
                }
            }
        }

        auto projectiles = m_manager.getGroup(Game::groupProjectiles);
        auto enemies = m_manager.getGroup(Game::groupEnemies);
        auto tiles = m_manager.getGroup(Game::groupColliders);

        for (auto &projectile : projectiles) {
            for (auto &tile : tiles) {
                if (tile->hasComponent<ColliderComponent>()) {
                    auto recA = projectile->getComponent<ColliderComponent>().collider;
                    auto recB = tile->getComponent<ColliderComponent>().collider;
                    if (Collision::AABB(recA, recB)) {

                        std::cout << "arrow x " << recA.x << "y " << recA.y <<std::endl;
                        std::cout << "probs with x " << recB.x << "y " << recB.y <<std::endl;

                        projectile->destroy();  // std::cout << "hit tileeeee" << std::endl;//TODO: make the arrow to bounce back
                    }
                }
            }
            for (auto &enemy : enemies) {
                if (enemy->hasComponent<ColliderComponent>()) {
                    if (Collision::AABB(projectile->getComponent<ColliderComponent>().collider,
                                        enemy->getComponent<ColliderComponent>().collider)) {
                        // TODO: generate enemy kill event, this will be the reaction:
                        enemy->getComponent<AnimationComponent>().setActive("die");
                    }
                }
                if (enemy->getComponent<AnimationComponent>().getActive()->isDone()) {
                    enemy->destroy();
                }
            }
        }
    }
};
