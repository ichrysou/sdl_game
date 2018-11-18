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

    TileComponent() = default;
    TileComponent(int x, int y, int w, int h, int id) {
        tileRect.x = x;
        tileRect.y = y;
        tileRect.w = w;
        tileRect.h = h;
        tileID = id;

        switch(id) {

        case 0: // rock
        case 1: // wall
        case 2: // water
            path = "../assets/tiles.png";
            break;

        default:
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
