#pragma once
#include "../Collision.h"
#include "ColliderComponent.h"
#include "ProjectileComponent.h"
#include "game.h"

class DeadEntitiesCleanupSystem {
   private:
    Manager &m_manager;
    bool attack_pressed = false;

   public:
    DeadEntitiesCleanupSystem(Manager &manager) : m_manager(manager) {}
    void init(){};
    void update() {
        auto enemies = m_manager.getGroup(Game::groupEnemies);

        for (auto &enemy : enemies) {
            if (enemy->hasComponent<DeadComponent>() && enemy->getComponent<AnimationComponent>().getActive()->isDone()) {
                if (!enemy->hasComponent<CleanupComponent>())
                    enemy->addComponent<CleanupComponent>(1000);
            }
        }
    }
};
