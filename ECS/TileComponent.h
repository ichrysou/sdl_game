#pragma once

#include "ECS.h"

#include "TransformComponent.h"
#include "SDL.h"


class TileComponent : public Component {
public:
    TransformComponent *transform;
    SpriteComponent *sprite;
    SDL_Rect tileRect;
    int tileID;
    char* path;
    int _rotation;
    TileComponent() = default;
    TileComponent(int x, int y, int w, int h, int id, int rotation) {
        tileRect.x = x;
        tileRect.y = y;
        tileRect.w = w;
        tileRect.h = h;
        tileID = id;
        _rotation = rotation;
        switch(id) {

        case 0: // rock
        case 1: // wall
        case 2: // water
        case 3: // grass
        default:
            path = "../assets/tiles2.png";
            break;
        }

    }

    void init() override
    {
        entity->addComponent<TransformComponent>(tileRect.x, tileRect.y, tileRect.w, tileRect.h, 2); // TODO: scale for tiles?
        entity->addComponent<SpriteComponent>(path);
        sprite = &entity->getComponent<SpriteComponent>();
        sprite->setSrcOffset(tileID);
   }

};
