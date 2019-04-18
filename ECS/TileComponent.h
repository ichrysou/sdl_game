#pragma once

#include "ECS.h"

#include "TransformComponent.h"
#include "SDL.h"

class TileComponent : public Component
{
  private:
    SDL_Texture *texture;
    Vector2D position;
    int m_scale;

  public:
    SDL_Rect srcRect, dstRect;
    TransformComponent *transform;
    SpriteComponent *sprite;
    SDL_Rect tileRect;
    int tileID;

    int _rotation;
    TileComponent() = default;
    ~TileComponent()
    {
        SDL_DestroyTexture(texture);
    }

    TileComponent(int srcX, int srcY, int x, int y, int w, int h, int rotation, const char *path, int scale)
    {
        m_scale = scale;
        texture = TextureManager::LoadTexture(path);
        srcRect.x = srcX;
        srcRect.y = srcY;
        srcRect.w = w;
        srcRect.h = h;
        dstRect.x = x * scale;
        dstRect.y = y * scale;
        dstRect.w = w * scale;
        dstRect.h = h * scale;
        position.x = x * scale;
        position.y = y * scale;
    }
    void update() override
    {
        dstRect.x = (position.x - Game::camera.x); // * m_scale;
        dstRect.y = (position.y - Game::camera.y); // * m_scale;
    }
    void draw() override
    {

        TextureManager::Draw(texture, srcRect, dstRect, SDL_FLIP_NONE);
        /*         entity->addComponent<TransformComponent>(tileRect.x, tileRect.y, tileRect.w, tileRect.h, 2); // TODO: scale for tiles?
        entity->addComponent<SpriteComponent>(path);
        sprite = &entity->getComponent<SpriteComponent>();
        sprite->setSrcOffset(tileID); */
    }
};
