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
    int speed;

  public:
    SDL_RendererFlip spriteflip = SDL_FLIP_NONE;
    bool m_animate; //TODO: they are specifically exported to be access from Keyboard Controller.. Find a better way
    const char *m_tag;
    SpriteComponent() = default;
    SpriteComponent(const char *path)
    {
        m_animate = false;
        setTex(path);
        m_tag = "none";
    }
    SpriteComponent(const char *tag, const char *path, bool animate = false)
    {
        m_animate = animate;
        if (tag == "knight")
        {
            Animation walk = Animation(1, 2, 100);
            Animation idle = Animation(0, 4, 100); //TODO: remove magic numbers
            animations.emplace("idle", idle);
            animations.emplace("walk", walk);
            play("idle");
        }
        setTex(path);
        m_tag = tag;
    }

    ~SpriteComponent()
    {
        SDL_DestroyTexture(texture);
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

            if (std::string(m_tag) == "skeleton")
            {
                speed = 20;
                int frameno = SDL_GetTicks() / speed; //actual
                frames = 10;
                srcRect.x = (srcRect.w + ((frameno % frames) * transform->width)) % (transform->width * frames);
                Vector2D skel_pos = transform->position;
                destRect.x = static_cast<int>(skel_pos.x) - Game::camera.x;
                destRect.y = static_cast<int>(skel_pos.y) - Game::camera.y;
            }
            else if (std::string(m_tag) == "knight")
            {
                int frameno = SDL_GetTicks() / speed; //actual
                srcRect.x = (srcRect.w + ((frameno % frames) * transform->width)) % (transform->width * frames);
                destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
                destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
            }
            else if (std::string(m_tag) == "thunder")
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

    void setTex(const char *path)
    {
        texture = TextureManager::LoadTexture(path);
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
