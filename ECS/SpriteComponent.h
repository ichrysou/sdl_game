#pragma once

#include <map>
#include "Components.h"
#include <SDL.h>
#include "../textureManager.h"
#include "Animation.h"

class SpriteComponent : public Component
{
  private:
    TransformComponent *transform;
    SDL_Texture *texture;

    SDL_Rect srcRect, destRect;
    int animIndex = 0;
    std::map<const char *, Animation> animations;
    int frames;
    int  speed;

  public:
    SDL_RendererFlip spriteflip = SDL_FLIP_NONE;
    bool m_animate; //TODO: they are specifically exported to be access from Keyboard Controller.. Find a better way
    std::string m_id;
    SpriteComponent() = default;

    SpriteComponent(std::string id)
    {
        m_animate = false;
        setTex(id);
        m_id = "none";
    }
    SpriteComponent(std::string id, bool animate = false)
    {
        m_animate = animate;
        if (id == "knight")
        {
            Animation walk = Animation(1, 2, 10);
            Animation idle = Animation(0, 4, 10); //TODO: remove magic numbers
            animations.emplace("idle", idle);
            animations.emplace("walk", walk);
            play("idle");
        }
        setTex(id);
        m_id = id;
    }

    ~SpriteComponent()
    {        
    }

    void init() override
    {

        transform = &entity->getComponent<TransformComponent>();
        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
        destRect.w = transform->scale * transform->width;
        destRect.h = transform->scale * transform->height;
    }

    void setSrcOffset(int x)             // TODO: remove
    {                                    //TODO: I don't like this interface.. Find a better way, change animation design pattern?
        srcRect.x = srcRect.w * (x % 4); // TODO: remove magic numbers from here
        srcRect.y = srcRect.h * (x / 4);
    }

    void update() override
    {
        // TODO: change animation design pattern
        //TODO: need to pass: number of frames
        if (m_animate)
        {

            if (m_id == "skeleton")
            {
                speed = 100; // TODO: rename speed to reverse speed
                int frameno = SDL_GetTicks() / speed; //actual
                frames = 10;
                srcRect.x = (srcRect.w + ((frameno % frames) * transform->width)) % (transform->width * frames);
                animIndex = 2;

                destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
                destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
            }
            else if (m_id == "knight")
            {
                speed = 100;
                frames = 4;
                int frameno = SDL_GetTicks() / speed; //actual
                srcRect.x = (srcRect.w + ((frameno % frames) * transform->width)) % (transform->width * frames);
                animIndex = 0;
                destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
                destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
            }
            else if (m_id == "thunder")
            { // TODO: make a ProjectileComponent of player for the thunder
                speed = 50;
                int frameno = SDL_GetTicks() / speed; //actual
                frames = 11;
                srcRect.x = (srcRect.w + ((frameno % frames) * transform->width)) % (transform->width * frames);
                destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
                destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
            }
        }
        srcRect.y = animIndex * transform->height;
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void setTex(std::string id)
    {
        texture = Game::assets->GetTexture(id);
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, spriteflip);
    }

    void play(const char *animName)
    {
        frames = animations[animName].frame;
        animIndex = animations[animName].index;
        speed = animations[animName].speed;
    }
};
