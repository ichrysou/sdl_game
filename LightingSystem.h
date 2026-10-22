#pragma once

#include <SDL.h>
#include <vector>

class LightingSystem
{
public:
    LightingSystem() = default;
    ~LightingSystem();

    LightingSystem(const LightingSystem &) = delete;
    LightingSystem &operator=(const LightingSystem &) = delete;

    void initialize(SDL_Renderer *renderer);
    void configureMap(int worldWidth, int worldHeight, int tileSize,
                     const std::vector<SDL_Rect> &occluders);
    void clearLights();
    void addLight(float x, float y, float radius, float intensity = 1.0f);
    void update();
    void render(const SDL_Rect &camera);
    void clean();

private:
    struct Light
    {
        float x;
        float y;
        float radius;
        float intensity;
    };

    bool hasLineOfSight(int startX, int startY, int targetX, int targetY) const;
    void ensureOverlaySize(int width, int height);

    SDL_Renderer *m_renderer = nullptr;
    SDL_Texture *m_overlay = nullptr;
    SDL_PixelFormat *m_pixelFormat = nullptr;
    int m_overlayWidth = 0;
    int m_overlayHeight = 0;
    bool m_overlayFailed = false;
    int m_worldWidth = 0;
    int m_worldHeight = 0;
    int m_tileSize = 32;
    int m_cellSize = 16;
    int m_gridWidth = 0;
    int m_gridHeight = 0;
    int m_blockerWidth = 0;
    int m_blockerHeight = 0;
    std::vector<unsigned char> m_blockers;
    std::vector<unsigned char> m_explored;
    std::vector<float> m_illumination;
    std::vector<Light> m_lights;
    std::vector<Uint32> m_pixels;
};
