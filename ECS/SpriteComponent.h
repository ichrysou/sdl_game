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
    bool m_visible = true;
    bool m_flickering = false;
    Uint32 m_flickerStartedAt = 0;
    Uint32 m_lastFlickerAt = 0;
    float m_renderScale = 1.0f;

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
    void setSrcFrame(int x, int y, int width, int height)
    {
        srcRect.x = x;
        srcRect.y = y;
        srcRect.w = width;
        srcRect.h = height;
    }

    void update() {
        const int entityWidth = transform->width * transform->scale;
        const int entityHeight = transform->height * transform->scale;
        destRect.w = static_cast<int>(entityWidth * m_renderScale);
        destRect.h = static_cast<int>(entityHeight * m_renderScale);
        destRect.x = static_cast<int>(transform->position.x - Game::camera.x) +
                     (entityWidth - destRect.w) / 2;
        destRect.y = static_cast<int>(transform->position.y - Game::camera.y) +
                     (entityHeight - destRect.h) / 2;

        if (!m_flickering) return;
        const Uint32 now = SDL_GetTicks();
        if (now - m_flickerStartedAt >= 1000) {
            m_flickering = false;
            m_visible = true;
        } else if (now - m_lastFlickerAt >= 100) {
            m_visible = !m_visible;
            m_lastFlickerAt = now;
        }
    }

    void startFlicker() {
        if (m_flickering) return;
        m_flickering = true;
        m_visible = false;
        m_flickerStartedAt = SDL_GetTicks();
        m_lastFlickerAt = m_flickerStartedAt;
    }

    void setRenderScale(float scale) {
        m_renderScale = scale;
    }

    void setTex(std::string id) {
        if (m_id == id) return;
        m_id = id;
        texture = Game::assets->GetTexture(id);
    }

    void draw() {
        if (m_visible)
            TextureManager::Draw(texture, srcRect, destRect, spriteflip, m_angle);
    }
};
