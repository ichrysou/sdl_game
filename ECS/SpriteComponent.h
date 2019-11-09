#pragma once

#include <SDL.h>
#include <map>
#include "../textureManager.h"


class SpriteComponent : public Component {
   private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;
    double m_angle = NULL;

   public:
    SDL_RendererFlip spriteflip = SDL_FLIP_NONE;

    std::string m_id;
    SpriteComponent() = default;

    SpriteComponent(std::string id) {

        setTex(id);
    }
    SpriteComponent(std::string id, double angle) {

        m_angle = angle;
        setTex(id);

    }

    ~SpriteComponent() {}

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
        destRect.w = transform->scale * transform->width;
        destRect.h = transform->scale * transform->height;
    }

    void setSrcOffset(int x)              // TODO: remove
    {
        srcRect.x = srcRect.w * (x % 4);  // TODO: remove magic numbers from here
        srcRect.y = srcRect.h * (x / 4);
    }
    void setSrcXY(int x, int y) // TODO: remove
    {
      // design pattern?
        srcRect.x = x;
        srcRect.y = y;
    }

    void update() {
        destRect.x = static_cast<int>(transform->position.x - Game::camera.x);
        destRect.y = static_cast<int>(transform->position.y - Game::camera.y);
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void setTex(std::string id) { m_id = id; texture = Game::assets->GetTexture(id); }

    void draw() { TextureManager::Draw(texture, srcRect, destRect, spriteflip, m_angle); }
};
