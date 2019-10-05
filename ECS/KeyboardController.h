#pragma once
#include "../game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
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
        if (sprite->m_id == "thunder")
        {
            if (Game::event.type == SDL_KEYDOWN)
            {
                switch (Game::event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    sprite->m_animate = true;
                    break;
                default:
                    break;
                }
            }
            else if (Game::event.type == SDL_KEYUP)
            {
                switch (Game::event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    sprite->setSrcOffset(0);
                    sprite->m_animate = false;
                    break;
                default:
                    break;
                }
            }
        }
        else
        {
            if (Game::event.type == SDL_KEYDOWN)
            {
                switch (Game::event.key.keysym.sym)
                {
                case SDLK_w:
                    transform->velocity.y = -1;
                    sprite->play("walk-up");
                    break;
                case SDLK_a: //TODO: fix movement, react on key press not key down?
                    sprite->play("walk");
                    transform->velocity.x = -1;
                    sprite->spriteflip = SDL_FLIP_HORIZONTAL;
                    break;
                case SDLK_s:
                    sprite->play("walk-down");
                    transform->velocity.y = 1;
                    break;
                case SDLK_d:
                    sprite->play("walk");
                    transform->velocity.x = 1;
                    break;
                    break;
                default:
                    break;
                }
            }
            if (Game::event.type == SDL_KEYUP)
            {
                switch (Game::event.key.keysym.sym)
                {
                case SDLK_w:
                    sprite->play("idle");
                    transform->velocity.y = 0;
                    break;
                case SDLK_a:
                    sprite->play("idle");
                    if (transform->velocity.x == -1)
                        transform->velocity.x = 0;
                    sprite->spriteflip = SDL_FLIP_NONE;
                    break;
                case SDLK_s:
                    sprite->play("idle");
                    transform->velocity.y = 0;
                    break;
                case SDLK_d:
                    sprite->play("idle");
                    if (transform->velocity.x == 1)
                        transform->velocity.x = 0;
                    break;
                case SDLK_ESCAPE:
                    Game::isRunning = false;
                    break;
                default:
                    break;
                }
            }
        }
    }
};
