#pragma once
#include <string>
#include "SDL.h"
#include "ECS.h"
#include "TransformComponent.h"
#include "game.h"
#include "textureManager.h"
#define DEBUG
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
    ColliderComponent(std::string t, int xpos, int ypos, int width, int height, int scale = 1)
    {
        tag = t;
        collider.x = xpos * scale;
        collider.y = ypos * scale;
        collider.w = width * scale;
        collider.h = height * scale;
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

#ifdef DEBUG
    void draw() {
        TextureManager::DrawSquare(collider);
    };
#endif

    void update() override
    {
        //TODO: rotate the colliders!
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
