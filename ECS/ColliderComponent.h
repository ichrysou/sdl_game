#pragma once
#include <string>
#include "SDL2/SDL.h"
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
    //    TransformComponent *transform;

    ColliderComponent(std::string t)
    {
        tag = t;
    }
    ColliderComponent(std::string t, int xpos, int ypos, int width, int height)
    {
        tag = t;
        collider.x = xpos * 2;
        collider.y = ypos * 2; //TODO: scaling issue and magic number hate
        collider.w = width;
        collider.h = height;
    }
    void init() override
    {
        /* if (!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>(); */
        /* entity->addGroup(Game::groupColliders); */
    }
    void update() override
    {
        if (entity->hasComponent<TransformComponent>())
        {
            auto transform = &entity->getComponent<TransformComponent>();
            collider.x = static_cast<int>(transform->position.x);
            collider.y = static_cast<int>(transform->position.y);
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
        }
    }
};
