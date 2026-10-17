#pragma once

#include "ECS.h"

#include "TransformComponent.h"
#include "SDL.h"
#include <algorithm>

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

    SDL_RendererFlip _flip;
    double _angle;
    TileComponent() = default;
    ~TileComponent()
    {
        SDL_DestroyTexture(texture);
    }

    TileComponent(int srcX, int srcY, int x, int y, int w, int h, int rotation, SDL_RendererFlip flip, const char *path, int scale)
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
        _flip = flip;
        _angle = (double)rotation;
    }
    void update() override
    {
        dstRect.x = (position.x - Game::camera.x); // * m_scale;
        dstRect.y = (position.y - Game::camera.y); // * m_scale;
    }
    void draw() override
    {
        TextureManager::Draw(texture, srcRect, dstRect, _flip, _angle);
        if (srcRect.x == 0 && srcRect.y == 128 && srcRect.w == 32 && srcRect.h == 32)
            drawTorchFlicker();
        /*         entity->addComponent<TransformComponent>(tileRect.x, tileRect.y, tileRect.w, tileRect.h, 2); // TODO: scale for tiles?
        entity->addComponent<SpriteComponent>(path);
        sprite = &entity->getComponent<SpriteComponent>();
        sprite->setSrcOffset(tileID); */
    }

  private:
    void drawTorchFlicker()
    {
        const int tileX = static_cast<int>(position.x / 32);
        const int tileY = static_cast<int>(position.y / 32);
        const int phase = (SDL_GetTicks() / 100 + tileX * 3 + tileY * 5) % 4;
        const int offsets[] = {0, 1, 0, -1};
        const int widths[] = {4, 3, 5, 3};
        const int heights[] = {7, 8, 6, 7};
        const float scaleX = dstRect.w / 32.0f;
        const float scaleY = dstRect.h / 32.0f;
        const int flameWidth = static_cast<int>(widths[phase] * scaleX);
        const int flameHeight = static_cast<int>(heights[phase] * scaleY);
        const int flameX = dstRect.x + static_cast<int>((16 + offsets[phase] - widths[phase] / 2) * scaleX);
        const int flameY = dstRect.y + static_cast<int>((12 - heights[phase]) * scaleY);
        SDL_BlendMode previousBlendMode;
        Uint8 previousRed, previousGreen, previousBlue, previousAlpha;
        SDL_GetRenderDrawBlendMode(Game::renderer, &previousBlendMode);
        SDL_GetRenderDrawColor(Game::renderer, &previousRed, &previousGreen,
                               &previousBlue, &previousAlpha);
        SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawColor(Game::renderer, 255, 105, 20, 110);
        SDL_Rect outerFlame{flameX, flameY, flameWidth, flameHeight};
        SDL_RenderFillRect(Game::renderer, &outerFlame);

        SDL_SetRenderDrawColor(Game::renderer, 255, 210, 70, 150);
        SDL_Rect innerFlame{
            flameX + static_cast<int>(scaleX),
            flameY + static_cast<int>(2 * scaleY),
            std::max(1, flameWidth - static_cast<int>(2 * scaleX)),
            std::max(1, flameHeight - static_cast<int>(3 * scaleY))};
        SDL_RenderFillRect(Game::renderer, &innerFlame);
        SDL_SetRenderDrawBlendMode(Game::renderer, previousBlendMode);
        SDL_SetRenderDrawColor(Game::renderer, previousRed, previousGreen,
                               previousBlue, previousAlpha);
    }
};
