#pragma once
#include "../game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
    SDL_Keycode m_last_pressed;

public:
    bool A = false;
    bool S = false;
    bool D = false;
    bool W = false;
    bool SPACE = false;
    bool at_least_one_pressed() { return A || W || S || D || SPACE; }
    SDL_Keycode last_key_pressed() { return m_last_pressed; }
    void init() override
    {
    }

    void update() override
    {
        if (Game::event.type == SDL_KEYDOWN)
        {
            m_last_pressed = Game::event.key.keysym.sym;
            switch (Game::event.key.keysym.sym)
            {

            case SDLK_SPACE:
                SPACE = true;
                break;
            case SDLK_w:
                W = true;
                break;
            case SDLK_a:
                A = true;
                break;
            case SDLK_s:
                S = true;
                break;
            case SDLK_d:
                D = true;
                break;
            default:
                break;
            }
        }
        if (Game::event.type == SDL_KEYUP)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_SPACE: //TODO: add release arrow event here
                SPACE = false;
                break;
            case SDLK_w:
                W = false;
                break;
            case SDLK_a:
                A = false;

                break;
            case SDLK_s:
                S = false;
                break;
            case SDLK_d:
                D = false;
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

;
