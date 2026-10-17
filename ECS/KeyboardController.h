#pragma once
#include "../game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
    SDL_Keycode m_last_pressed = SDLK_UNKNOWN;

public:
    bool A = false;
    bool S = false;
    bool D = false;
    bool W = false;
    bool SPACE = false;
    bool spacePressedThisFrame = false;
    bool spaceReleasedThisFrame = false;
    bool at_least_one_pressed() { return A || W || S || D || SPACE; }
    SDL_Keycode last_key_pressed() { return m_last_pressed; }
    void init() override
    {
    }

    void handleEvent(const SDL_Event &event)
    {
        if (event.type == SDL_KEYDOWN)
        {
            m_last_pressed = event.key.keysym.sym;
            switch (event.key.keysym.sym)
            {

            case SDLK_SPACE:
                SPACE = true;
                if (event.key.repeat == 0) spacePressedThisFrame = true;
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
            case SDLK_ESCAPE:
                Game::isRunning = false;
                break;
            default:
                break;
            }
        }
        if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_SPACE: //TODO: add release arrow event here
                SPACE = false;
                spaceReleasedThisFrame = true;
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
            default:
                break;
            }
        }
    }

    void clearTransientInput()
    {
        spacePressedThisFrame = false;
        spaceReleasedThisFrame = false;
    }
}

;
