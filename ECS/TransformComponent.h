#pragma once

#include "../Vector2D.h"

class
    TransformComponent : public Component
{
public:
    Vector2D position;
    Vector2D velocity;
    Vector2D orientation;

    int height = 32;
    int width = 32;
    int scale = 2;
    int speed = 5;

    TransformComponent()
    {
        position.Zero();
    }

    TransformComponent(int sc)
    {
        position.Zero();
        scale = sc;
    }

    TransformComponent(float x, float y)
    {
        position.x = x;
        position.y = y;
    }
    TransformComponent(float x, float y, int sc)
    {
        position.x = x;
        position.y = y;
        scale = sc;
    }

    TransformComponent(float x, float y, int h, int w, int sc)
    {
        position.x = x;
        position.y = y;
        height = h;
        width = w;
        scale = sc;
    }

    void init() override
    {
        velocity.Zero();
        orientation.x = 1;
        orientation.y = 0;
    }

    void update() override
    {
        /* TODO: can I use overloaded vector operators? */
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
    }
};
