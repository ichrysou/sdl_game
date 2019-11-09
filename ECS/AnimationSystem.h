#pragma once
#include "game.h"
#include "Components.h"

class AnimationSystem {
   private:
    Manager &m_manager;
    bool attack_pressed = false;

   public:
    AnimationSystem(Manager &manager) : m_manager(manager) {}
    void init() {
    };
    void update() {
        /* for (auto& entity : m_manager.getEntities<AnimationComponent>()) { */
        /*     Animation *anim_comp = entity->getComponent<AnimationComponent>(); */
        /*     if (anim_comp->getActive()) */
        /* } */
    };
};
