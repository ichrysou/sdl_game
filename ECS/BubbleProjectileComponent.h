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

        constexpr int segments = 32;
        SDL_Vertex vertices[segments + 1];
        int indices[segments * 3];
        vertices[0].position = SDL_FPoint{centerX, centerY};
        vertices[0].color = SDL_Color{72, 196, 230, 92};
        vertices[0].tex_coord = SDL_FPoint{0.0f, 0.0f};
        SDL_Point outline[segments + 1];
        for (int i = 0; i < segments; ++i)
        {
            const float angle = 6.2831853f * i / segments;
            const float x = std::cos(angle);
            const float y = std::sin(angle);
            vertices[i + 1].position =
                SDL_FPoint{centerX + x * radius * 2.0f,
                           centerY + y * radius * 2.0f};
            vertices[i + 1].color = SDL_Color{35, 150, 220, 0};
            vertices[i + 1].tex_coord = SDL_FPoint{0.0f, 0.0f};
            indices[i * 3] = 0;
            indices[i * 3 + 1] = i + 1;
            indices[i * 3 + 2] = (i + 1) % segments + 1;
            outline[i] = SDL_Point{
                static_cast<int>(centerX + x * radius),
                static_cast<int>(centerY + y * radius)};
        }
        indices[segments * 3 - 1] = 1;
        outline[segments] = outline[0];
        SDL_RenderGeometry(Game::renderer, nullptr, vertices, segments + 1,
                           indices, segments * 3);

        SDL_SetRenderDrawColor(Game::renderer, 230, 255, 255, 225);
        SDL_RenderDrawLines(Game::renderer, outline, segments + 1);
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
    TransformComponent *m_transform = nullptr;
    Vector2D m_direction;
    int m_diameter;
    float m_distance = 0.0f;
};
