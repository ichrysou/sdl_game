#pragma once

#include "Components.h"
#include "../Vector2D.h"
class ProjectileComponent : public Component
{
public:
    ProjectileComponent(int range, int speed, Vector2D velocity) : m_range(range), m_speed(speed), m_distance(0), m_velocity(velocity)
    {
    }
    ~ProjectileComponent() {}

    void init() override
    {
        m_transform = &entity->getComponent<TransformComponent>();
        m_transform->velocity = m_velocity;
    };
    void update() override
    {
        m_distance += m_speed;
        if (m_distance > m_range)
        {
            std::cout << "out of range" << std::endl;
            entity->destroy();
        }
        else if (m_transform->position.x > Game::camera.x + Game::camera.w ||
                 m_transform->position.x < Game::camera.x ||
                 m_transform->position.y > Game::camera.y + Game::camera.h ||
                 m_transform->position.y < Game::camera.y)
        {
            std::cout << "out of range" << std::endl;
            entity->destroy();
        }
    };

private:
    TransformComponent *m_transform;
    int m_range;
    int m_speed;
    int m_distance;
    Vector2D m_velocity;
};