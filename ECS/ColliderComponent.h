#pragma once
#include <string>
#include "SDL.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "game.h"

class ColliderComponent : public Component
{
public:
    SDL_Rect collider;
    // will allow us in the future to determine the action
    // based on the items that collide
    std::string tag;
    TransformComponent *transform;

    ColliderComponent(std::string t) {
        tag = t;
    }
    void init() override {
        if(!entity->hasComponent<TransformComponent>()) {
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();
        Game::colliders.push_back(this);
    }
    void update() override {
        collider.x = static_cast<int>(transform->position.x);
        collider.y = static_cast<int>(transform->position.y);
        collider.w = transform->width * transform->scale;
        collider.h = transform->height * transform->scale;
    }
};
