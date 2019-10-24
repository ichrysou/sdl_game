#pragma once
#include "Components.h"
#include "game.h"

class PlayerMovementSystem
{
private:
    Manager &m_manager;
    int8_t vel_w = -1;
    int8_t vel_a = -1;
    int8_t vel_s = 1;
    int8_t vel_d = 1;

private:
    void animate_movement(Entity *player)
    {
        auto animation = &player->getComponent<AnimationComponent>();
        auto transform = &player->getComponent<TransformComponent>();
        auto sprite = &player->getComponent<SpriteComponent>();
        auto zero = Vector2D(0, 0);
        auto up = Vector2D(0, vel_w);
        auto down = Vector2D(0, vel_s);
        auto left = Vector2D(vel_a, 0);
        auto right = Vector2D(vel_d, 0);
        if (transform->velocity == zero)
        {
            if (transform->orientation == left)
            {
                animation->setActive("idle");
                sprite->spriteflip = SDL_FLIP_HORIZONTAL;
            }
            else if (transform->orientation == right)
            {
                animation->setActive("idle");
                sprite->spriteflip = SDL_FLIP_NONE;
            }
            else if (transform->orientation == up)
            {
                animation->setActive("idle-up");
            }
            else if (transform->orientation == down)
            {
                animation->setActive("idle-down");
            }
        }
        else
        {
            if (transform->velocity.x == vel_d)
            {
                animation->setActive("walk");
                sprite->spriteflip = SDL_FLIP_NONE;
            }
            else if (transform->velocity.x == vel_a)
            {
                animation->setActive("walk");
                if (transform->velocity.y == 0)
                    sprite->spriteflip = SDL_FLIP_HORIZONTAL;
            }
            if (transform->velocity.y == vel_w)
            {
                animation->setActive("walk-up");
            }
            else if (transform->velocity.y == vel_s)
            {
                animation->setActive("walk-down");
            }
        }
    }
    void move_player(Entity *player)
    {
        auto sprite = &player->getComponent<SpriteComponent>();
        auto transform = &player->getComponent<TransformComponent>();
        auto keyboard = &player->getComponent<KeyboardController>();

        transform->velocity.x = static_cast<float>((keyboard->A ? vel_a : 0) + (keyboard->D ? vel_d : 0));
        transform->velocity.y = static_cast<float>((keyboard->S ? vel_s : 0) + (keyboard->W ? vel_w : 0));
        switch (keyboard->last_key_pressed())
        {
        case SDLK_w:
            transform->orientation.y = vel_w;
            transform->orientation.x = 0;
            break;
        case SDLK_a:
            transform->orientation.x = vel_a;
            transform->orientation.y = 0;
            break;
        case SDLK_s:
            transform->orientation.y = vel_s;
            transform->orientation.x = 0;
            break;
        case SDLK_d:
            transform->orientation.x = vel_d;
            transform->orientation.y = 0;
            break;
        default:
            break;
        }
    };

public:
    PlayerMovementSystem(Manager &manager) : m_manager(manager)
    {
    }
    void update()
    {
        auto players = m_manager.getGroup(Game::groupPlayers);
        for (auto &player : players)
        {
            move_player(player);
            animate_movement(player);
        }
    }
};