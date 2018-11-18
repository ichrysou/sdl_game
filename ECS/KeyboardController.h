#pragma once
#include "../game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component {
public:
    TransformComponent *transform;
    SpriteComponent *sprite;
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override
    {

        if(Game::event.type == SDL_KEYDOWN)
        {
            switch(Game::event.key.keysym.sym)
            {
            case SDLK_w:
                transform->velocity.y = -1;
                break;
            case SDLK_a://TODO: fix movement, react on key press not key down?
                transform->velocity.x = -1;
                break;
            case SDLK_s:
                transform->velocity.y = 1;
                break;
            case SDLK_d:
                transform->velocity.x = 1;
                break;
            default:
                break;
            }
        }
        if(Game::event.type == SDL_KEYUP)
        {
            switch(Game::event.key.keysym.sym)
            {
            case SDLK_w:
                transform->velocity.y = 0;
                break;
            case SDLK_a:
                if (transform->velocity.x == -1)            
                    transform->velocity.x = 0;
                break;
            case SDLK_s:
                transform->velocity.y = 0;
                break;
            case SDLK_d:
                if (transform->velocity.x == 1)            
                    transform->velocity.x = 0;
                break;
            default:
                break;
            }
        }
    }
};

