#pragma once

#include "Components.h"
#include <SDL.h>
#include "../textureManager.h"

class SpriteComponent : public Component {
   private:
    TransformComponent *transform;
    SDL_Texture *texture;
    bool m_animate;
    SDL_Rect srcRect, destRect;
    const char *m_tag;

    
public:
    SpriteComponent() = default;
    SpriteComponent(const char *tag, const char *path, bool animate = false) {
        setTex(path);
        m_animate = animate;
        m_tag = tag;
    }
    SpriteComponent(const char *path, bool animate = false) {
        setTex(path);
        m_animate = animate;
    }
    ~SpriteComponent() {
        SDL_DestroyTexture(texture);
    }
    void init() override {

        transform = &entity->getComponent<TransformComponent>();
        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
        destRect.w = transform->scale * transform->width;
        destRect.h = transform->scale * transform->height;
    }

    void setSrcOffset(int x) { //TODO: I don't like this interface.. Find a better way
        srcRect.x = srcRect.w * x;
    }

    void update() override {
        //TODO: need to pass: number of frames
        if (m_animate) {
            int no_of_frames;
            int frame_width;
            if(std::string(m_tag) == "skeleton") {
                no_of_frames = 10;
                frame_width = transform->width;
            }  else if (std::string(m_tag) == "knight") {
                no_of_frames = 2;
                frame_width = transform->width;
            } 
            int frameno =  SDL_GetTicks() / 200;
            srcRect.x = (srcRect.w + ((frameno % no_of_frames) * frame_width)) % (frame_width * no_of_frames);
        }
 
        destRect.x = static_cast<int>(transform->position.x);
        destRect.y = static_cast<int>(transform->position.y);
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }


    void setTex(const char *path) {
        texture = TextureManager::LoadTexture(path);
    }

    void draw() override {
        TextureManager::Draw(texture, srcRect, destRect);
    }

};
