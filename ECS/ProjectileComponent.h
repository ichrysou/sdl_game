#pragma once

#include "Components.h"

class ProjectileComponent : public Component
{
public:
    ProjectileComponent(int range, int speed) : m_range(range), m_speed(speed), m_distance(0)
    {
    }
    ~ProjectileComponent() {}

    void init() override
    {
        m_transform = &entity->getComponent<TransformComponent>();
    };
    void update() override
    {
        m_distance += m_speed;
        if (m_distance > m_range)
        {
            entity->destroy();
        }
        else if (m_transform->position.x > Game::camera.x + Game::camera.w ||
                 m_transform->position.x < Game::camera.x ||
                 m_transform->position.y > Game::camera.y + Game::camera.h ||
                 m_transform->position.y < Game::camera.y)
        {
            entity->destroy();
        }
    };

private:
    TransformComponent *m_transform;
    int m_range;
    int m_speed;
    int m_distance;
};