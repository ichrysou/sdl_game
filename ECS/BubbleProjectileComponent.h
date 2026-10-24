#pragma once

#include <algorithm>
#include <cmath>
#include "ECS.h"
#include "TransformComponent.h"
#include "../game.h"

class BubbleProjectileComponent : public Component
{
public:
    BubbleProjectileComponent(Vector2D direction, int diameter)
        : m_direction(direction), m_diameter(diameter)
    {
        const float length = std::sqrt(m_direction.x * m_direction.x +
                                       m_direction.y * m_direction.y);
        if (length > 0.0f)
        {
            m_direction.x /= length;
            m_direction.y /= length;
        }
    }

    void init() override
    {
        m_transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
        constexpr float speed = 2.5f;
        constexpr float range = 520.0f;
        m_transform->position.x += m_direction.x * speed;
        m_transform->position.y += m_direction.y * speed;
        m_distance += speed;
        if (m_distance >= range)
            entity->destroy();
    }

    void draw() override
    {
        const float centerX = m_transform->position.x - Game::camera.x +
                             m_diameter * 0.5f;
        const float centerY = m_transform->position.y - Game::camera.y +
                              m_diameter * 0.5f;
        const int radius = m_diameter / 2;

        SDL_BlendMode previousBlendMode;
        Uint8 previousR, previousG, previousB, previousA;
        SDL_GetRenderDrawBlendMode(Game::renderer, &previousBlendMode);
        SDL_GetRenderDrawColor(Game::renderer, &previousR, &previousG,
                               &previousB, &previousA);
        SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);

        drawFilledCircle(centerX, centerY, radius * 3, 28, 105, 190, 18);
        drawFilledCircle(centerX, centerY, radius * 2, 35, 150, 220, 30);
        drawFilledCircle(centerX, centerY, radius, 72, 196, 230, 88);
        drawCircleOutline(centerX, centerY, radius, 160, 240, 255, 210);

        SDL_SetRenderDrawColor(Game::renderer, 230, 255, 255, 225);
        SDL_RenderDrawLine(Game::renderer,
                           static_cast<int>(centerX - radius * 0.35f),
                           static_cast<int>(centerY - radius * 0.35f),
                           static_cast<int>(centerX + radius * 0.05f),
                           static_cast<int>(centerY - radius * 0.35f));

        SDL_SetRenderDrawColor(Game::renderer, previousR, previousG, previousB,
                               previousA);
        SDL_SetRenderDrawBlendMode(Game::renderer, previousBlendMode);
    }

    Vector2D getLightPosition() const
    {
        return Vector2D(m_transform->position.x + m_diameter * 0.5f,
                        m_transform->position.y + m_diameter * 0.5f);
    }

    float getLightRadius() const
    {
        return 104.0f + m_diameter * 4.0f;
    }

    float getLightIntensity() const
    {
        return std::min(1.0f, 0.55f + m_diameter * 0.015f);
    }

private:
    void drawFilledCircle(float centerX, float centerY, int radius,
                          Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
    {
        SDL_SetRenderDrawColor(Game::renderer, r, g, b, a);
        for (int y = -radius; y <= radius; ++y)
        {
            const int x = static_cast<int>(
                std::sqrt(static_cast<float>(radius * radius - y * y)));
            SDL_RenderDrawLine(Game::renderer,
                               static_cast<int>(centerX) - x,
                               static_cast<int>(centerY) + y,
                               static_cast<int>(centerX) + x,
                               static_cast<int>(centerY) + y);
        }
    }

    void drawCircleOutline(float centerX, float centerY, int radius,
                           Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
    {
        SDL_SetRenderDrawColor(Game::renderer, r, g, b, a);
        for (int y = -radius; y <= radius; ++y)
        {
            const int x = static_cast<int>(
                std::sqrt(static_cast<float>(radius * radius - y * y)));
            SDL_RenderDrawPoint(Game::renderer, static_cast<int>(centerX) - x,
                                static_cast<int>(centerY) + y);
            SDL_RenderDrawPoint(Game::renderer, static_cast<int>(centerX) + x,
                                static_cast<int>(centerY) + y);
        }
    }

    TransformComponent *m_transform = nullptr;
    Vector2D m_direction;
    int m_diameter;
    float m_distance = 0.0f;
};
