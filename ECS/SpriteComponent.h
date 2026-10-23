#pragma once

#include <SDL.h>
#include <algorithm>
#include <cmath>
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
    Uint32 m_thunderStartedAt = 0;
    bool m_thunderCasting = false;
    Vector2D m_thunderDirection;
    float m_renderScale = 1.0f;

    float thunderProgress() const {
        return std::min(1.0f, (SDL_GetTicks() - m_thunderStartedAt) / 420.0f);
    }

    float thunderStaffAngle(float progress) const {
        const float aim = std::atan2(m_thunderDirection.y, m_thunderDirection.x);
        if (progress < 0.42f)
            return aim - 1.10f + progress / 0.42f * 1.10f;
        if (progress < 0.72f)
            return aim;
        return aim + (progress - 0.72f) / 0.28f * 0.16f;
    }

    Vector2D staffTipAt(float progress, float centerX, float centerY,
                        float width, float height) const {
        const float handX = centerX +
                            (m_thunderDirection.y == 0.0f
                                 ? m_thunderDirection.x * width * 0.37f
                                 : width * 0.28f);
        const float handY = centerY +
                            (m_thunderDirection.y == 0.0f
                                 ? height * 0.12f
                                 : m_thunderDirection.y * height * 0.08f);
        const float angle = thunderStaffAngle(progress);
        const float length = std::max(width, height) * 0.82f;
        return Vector2D(handX + std::cos(angle) * length,
                        handY + std::sin(angle) * length);
    }

    void drawThunderStaff() {
        const float progress = thunderProgress();
        const float centerX = destRect.x + destRect.w * 0.5f;
        const float centerY = destRect.y + destRect.h * 0.5f;
        const float handX = centerX +
                            (m_thunderDirection.y == 0.0f
                                 ? m_thunderDirection.x * destRect.w * 0.37f
                                 : destRect.w * 0.28f);
        const float handY = centerY +
                            (m_thunderDirection.y == 0.0f
                                 ? destRect.h * 0.12f
                                 : m_thunderDirection.y * destRect.h * 0.08f);
        const Vector2D tip = staffTipAt(progress, centerX, centerY,
                                        static_cast<float>(destRect.w),
                                        static_cast<float>(destRect.h));
        const int x1 = static_cast<int>(handX);
        const int y1 = static_cast<int>(handY);
        const int x2 = static_cast<int>(tip.x);
        const int y2 = static_cast<int>(tip.y);
        const float perpendicularX = -std::sin(thunderStaffAngle(progress));
        const float perpendicularY = std::cos(thunderStaffAngle(progress));

        SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);
        if (progress > 0.20f && progress < 0.78f) {
            const float trailStart = std::max(0.0f, progress - 0.22f);
            Vector2D previous = staffTipAt(trailStart, centerX, centerY,
                                           static_cast<float>(destRect.w),
                                           static_cast<float>(destRect.h));
            for (int sample = 1; sample <= 5; ++sample) {
                const float t = trailStart + (progress - trailStart) * sample / 5.0f;
                const Vector2D current = staffTipAt(t, centerX, centerY,
                                                    static_cast<float>(destRect.w),
                                                    static_cast<float>(destRect.h));
                const Uint8 alpha = static_cast<Uint8>(24 + sample * 15);
                SDL_SetRenderDrawColor(Game::renderer, 42, 166, 196, alpha);
                SDL_RenderDrawLine(Game::renderer, static_cast<int>(previous.x),
                                  static_cast<int>(previous.y), static_cast<int>(current.x),
                                  static_cast<int>(current.y));
                previous = current;
            }
        }

        SDL_SetRenderDrawColor(Game::renderer, 42, 166, 196, 90);
        SDL_RenderDrawLine(Game::renderer, x1 - 2, y1, x2 - 2, y2);
        SDL_RenderDrawLine(Game::renderer, x1 + 2, y1, x2 + 2, y2);
        SDL_SetRenderDrawColor(Game::renderer, 12, 22, 38, 255);
        SDL_RenderDrawLine(Game::renderer, x1 - 1, y1, x2 - 1, y2);
        SDL_RenderDrawLine(Game::renderer, x1 + 1, y1, x2 + 1, y2);
        SDL_SetRenderDrawColor(Game::renderer, 180, 126, 52, 255);
        SDL_RenderDrawLine(Game::renderer, x1, y1, x2, y2);
        SDL_SetRenderDrawColor(Game::renderer, 244, 200, 104, 255);
        SDL_RenderDrawLine(Game::renderer, x1, y1, x2, y2);
        SDL_SetRenderDrawColor(Game::renderer, 255, 232, 160, 255);
        SDL_RenderDrawLine(Game::renderer,
                          x1 + static_cast<int>(perpendicularX),
                          y1 + static_cast<int>(perpendicularY),
                          x2 + static_cast<int>(perpendicularX),
                          y2 + static_cast<int>(perpendicularY));

        const int glowSize = progress > 0.40f && progress < 0.67f ? 11 : 7;
        SDL_Rect glow{x2 - glowSize / 2, y2 - glowSize / 2, glowSize, glowSize};
        SDL_SetRenderDrawColor(Game::renderer, 42, 166, 196, 75);
        SDL_RenderFillRect(Game::renderer, &glow);
        SDL_SetRenderDrawColor(Game::renderer, 162, 236, 224, 220);
        SDL_RenderDrawLine(Game::renderer, x2 - 4, y2, x2 + 4, y2);
        SDL_RenderDrawLine(Game::renderer, x2, y2 - 4, x2, y2 + 4);
        SDL_Rect crystal{x2 - 1, y2 - 1, 3, 3};
        SDL_SetRenderDrawColor(Game::renderer, 42, 166, 196, 255);
        SDL_RenderFillRect(Game::renderer, &crystal);
        SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
        SDL_RenderDrawPoint(Game::renderer, x2, y2);
        if (progress > 0.28f && progress < 0.72f) {
            const Uint32 now = SDL_GetTicks();
            for (int spark = 0; spark < 4; ++spark) {
                const float phase = now * 0.018f + spark * 1.5708f;
                const int sparkX = x2 + static_cast<int>(std::cos(phase) * (5 + spark % 2 * 2));
                const int sparkY = y2 + static_cast<int>(std::sin(phase) * (5 + spark % 2 * 2));
                SDL_SetRenderDrawColor(Game::renderer, 162, 236, 224, 210);
                SDL_RenderDrawPoint(Game::renderer, sparkX, sparkY);
            }
        }
        SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_NONE);
    }

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

    void startThunderCast(Vector2D direction) {
        m_thunderCasting = true;
        m_thunderStartedAt = SDL_GetTicks();
        m_thunderDirection = direction;
    }

    bool isThunderCasting() {
        return m_thunderCasting &&
               SDL_GetTicks() - m_thunderStartedAt < 420;
    }

    Vector2D getThunderStaffTipWorld(float progress) const {
        const float centerX = transform->position.x + transform->width * transform->scale * 0.5f;
        const float centerY = transform->position.y + transform->height * transform->scale * 0.5f;
        return staffTipAt(progress, centerX, centerY,
                          static_cast<float>(transform->width * transform->scale),
                          static_cast<float>(transform->height * transform->scale));
    }

    float getThunderCastProgress() const {
        return thunderProgress();
    }

    bool finishThunderCast() {
        if (!m_thunderCasting || isThunderCasting()) return false;
        m_thunderCasting = false;
        return true;
    }

    void setTex(std::string id) {
        if (m_id == id) return;
        m_id = id;
        texture = Game::assets->GetTexture(id);
    }

    void draw() {
        if (!m_visible) return;

        const bool thunderCasting = isThunderCasting();
        if (m_id == "thunder") {
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
            SDL_SetTextureColorMod(texture, 70, 175, 255);
            SDL_SetTextureAlphaMod(texture, 70);
            SDL_Rect glow = destRect;
            glow.x -= destRect.w / 4;
            glow.y -= destRect.h / 4;
            glow.w += destRect.w / 2;
            glow.h += destRect.h / 2;
            TextureManager::Draw(texture, srcRect, glow, spriteflip, m_angle);

            SDL_SetTextureAlphaMod(texture, 135);
            glow = destRect;
            glow.x -= destRect.w / 10;
            glow.y -= destRect.h / 10;
            glow.w += destRect.w / 5;
            glow.h += destRect.h / 5;
            TextureManager::Draw(texture, srcRect, glow, spriteflip, m_angle);

            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            SDL_SetTextureColorMod(texture, 255, 255, 255);
            SDL_SetTextureAlphaMod(texture, 255);
        }

        TextureManager::Draw(texture, srcRect, destRect, spriteflip, m_angle);
        if (thunderCasting && m_id == "hero-cast")
            drawThunderStaff();
    }
};
