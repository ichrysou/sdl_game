#include "LightingSystem.h"

#include <algorithm>
#include <cmath>
#include <iostream>

namespace
{
constexpr unsigned char unexploredAlpha = 255;
constexpr unsigned char exploredAlpha = 205;
}

LightingSystem::~LightingSystem()
{
    clean();
}

void LightingSystem::initialize(SDL_Renderer *renderer)
{
    clean();
    m_renderer = renderer;
    m_pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
    if (!m_pixelFormat)
        std::cerr << "Lighting pixel format creation failed: " << SDL_GetError() << std::endl;
}

void LightingSystem::configureMap(int worldWidth, int worldHeight, int tileSize,
                                  const std::vector<SDL_Rect> &occluders)
{
    m_worldWidth = std::max(0, worldWidth);
    m_worldHeight = std::max(0, worldHeight);
    m_tileSize = std::max(1, tileSize);
    m_gridWidth = (m_worldWidth + m_cellSize - 1) / m_cellSize;
    m_gridHeight = (m_worldHeight + m_cellSize - 1) / m_cellSize;
    m_blockerWidth = (m_worldWidth + m_tileSize - 1) / m_tileSize;
    m_blockerHeight = (m_worldHeight + m_tileSize - 1) / m_tileSize;

    m_blockers.assign(static_cast<std::size_t>(m_blockerWidth) * m_blockerHeight, 0);
    m_explored.assign(static_cast<std::size_t>(m_gridWidth) * m_gridHeight, 0);
    m_illumination.assign(static_cast<std::size_t>(m_gridWidth) * m_gridHeight, 0.0f);

    for (const SDL_Rect &rect : occluders)
    {
        const int left = std::max(0, rect.x / m_tileSize);
        const int top = std::max(0, rect.y / m_tileSize);
        const int right = std::min(m_blockerWidth, (rect.x + rect.w + m_tileSize - 1) / m_tileSize);
        const int bottom = std::min(m_blockerHeight, (rect.y + rect.h + m_tileSize - 1) / m_tileSize);
        for (int y = top; y < bottom; ++y)
            for (int x = left; x < right; ++x)
                m_blockers[static_cast<std::size_t>(y) * m_blockerWidth + x] = 1;
    }
}

void LightingSystem::clearLights()
{
    m_lights.clear();
}

void LightingSystem::addLight(float x, float y, float radius, float intensity)
{
    if (radius > 0.0f && intensity > 0.0f)
        m_lights.push_back({x, y, radius, std::min(1.0f, intensity)});
}

bool LightingSystem::hasLineOfSight(int startX, int startY, int targetX, int targetY) const
{
    int x = startX;
    int y = startY;
    const int dx = std::abs(targetX - startX);
    const int sx = startX < targetX ? 1 : -1;
    const int dy = -std::abs(targetY - startY);
    const int sy = startY < targetY ? 1 : -1;
    int error = dx + dy;

    while (x != targetX || y != targetY)
    {
        const int doubledError = 2 * error;
        if (doubledError >= dy)
        {
            error += dy;
            x += sx;
        }
        if (doubledError <= dx)
        {
            error += dx;
            y += sy;
        }
        if (x == targetX && y == targetY)
            break;
        if (x < 0 || y < 0 || x >= m_blockerWidth || y >= m_blockerHeight ||
            m_blockers[static_cast<std::size_t>(y) * m_blockerWidth + x])
            return false;
    }
    return true;
}

void LightingSystem::update()
{
    if (m_worldWidth <= 0 || m_worldHeight <= 0)
        return;

    std::fill(m_illumination.begin(), m_illumination.end(), 0.0f);
    for (const Light &light : m_lights)
    {
        const int left = std::max(0, static_cast<int>((light.x - light.radius) / m_cellSize));
        const int top = std::max(0, static_cast<int>((light.y - light.radius) / m_cellSize));
        const int right = std::min(m_gridWidth - 1,
                                   static_cast<int>((light.x + light.radius) / m_cellSize));
        const int bottom = std::min(m_gridHeight - 1,
                                    static_cast<int>((light.y + light.radius) / m_cellSize));
        const int sourceTileX = static_cast<int>(light.x) / m_tileSize;
        const int sourceTileY = static_cast<int>(light.y) / m_tileSize;

        for (int y = top; y <= bottom; ++y)
        {
            for (int x = left; x <= right; ++x)
            {
                const float worldX = x * m_cellSize + m_cellSize * 0.5f;
                const float worldY = y * m_cellSize + m_cellSize * 0.5f;
                const float dx = worldX - light.x;
                const float dy = worldY - light.y;
                const float distance = std::sqrt(dx * dx + dy * dy);
                if (distance >= light.radius)
                    continue;

                const int targetTileX = static_cast<int>(worldX) / m_tileSize;
                const int targetTileY = static_cast<int>(worldY) / m_tileSize;
                if (!hasLineOfSight(sourceTileX, sourceTileY, targetTileX, targetTileY))
                    continue;

                const float falloff = 1.0f - distance / light.radius;
                const float strength = light.intensity * falloff * falloff;
                const std::size_t index = static_cast<std::size_t>(y) * m_gridWidth + x;
                m_illumination[index] = std::max(m_illumination[index], strength);
                if (strength > 0.015f)
                    m_explored[index] = 1;
            }
        }
    }
}

void LightingSystem::ensureOverlaySize(int width, int height)
{
    if (width == m_overlayWidth && height == m_overlayHeight &&
        (m_overlay || m_overlayFailed))
        return;

    if (m_overlay)
        SDL_DestroyTexture(m_overlay);
    m_overlay = nullptr;
    m_overlayWidth = width;
    m_overlayHeight = height;
    m_overlayFailed = false;
    if (!m_renderer || width <= 0 || height <= 0)
        return;

    m_overlay = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA32,
                                  SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!m_overlay)
    {
        m_overlayFailed = true;
        std::cerr << "Lighting overlay creation failed: " << SDL_GetError() << std::endl;
        return;
    }
    if (SDL_SetTextureBlendMode(m_overlay, SDL_BLENDMODE_BLEND) != 0)
        std::cerr << "Lighting overlay blend setup failed: " << SDL_GetError() << std::endl;
}

void LightingSystem::render(const SDL_Rect &camera)
{
    if (!m_renderer || !m_pixelFormat || m_gridWidth <= 0 || m_gridHeight <= 0)
        return;

    const int width = (camera.w + m_cellSize - 1) / m_cellSize;
    const int height = (camera.h + m_cellSize - 1) / m_cellSize;
    ensureOverlaySize(width, height);
    if (!m_overlay)
        return;

    m_pixels.resize(static_cast<std::size_t>(width) * height);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const int worldX = camera.x + x * m_cellSize + m_cellSize / 2;
            const int worldY = camera.y + y * m_cellSize + m_cellSize / 2;
            const int gridX = worldX / m_cellSize;
            const int gridY = worldY / m_cellSize;
            unsigned char alpha = unexploredAlpha;
            if (gridX >= 0 && gridY >= 0 && gridX < m_gridWidth && gridY < m_gridHeight)
            {
                const std::size_t index = static_cast<std::size_t>(gridY) * m_gridWidth + gridX;
                if (m_explored[index])
                {
                    const float strength = std::min(1.0f, m_illumination[index]);
                    alpha = static_cast<unsigned char>(
                        std::max(0.0f, exploredAlpha * (1.0f - strength)));
                }
            }
            m_pixels[static_cast<std::size_t>(y) * width + x] =
                SDL_MapRGBA(m_pixelFormat, 6, 9, 18, alpha);
        }
    }

    const int pitch = width * static_cast<int>(sizeof(Uint32));
    if (SDL_UpdateTexture(m_overlay, nullptr, m_pixels.data(), pitch) != 0)
    {
        std::cerr << "Lighting overlay update failed: " << SDL_GetError() << std::endl;
        return;
    }

    const SDL_Rect destination{0, 0, camera.w, camera.h};
    if (SDL_RenderCopy(m_renderer, m_overlay, nullptr, &destination) != 0)
        std::cerr << "Lighting overlay render failed: " << SDL_GetError() << std::endl;
}

void LightingSystem::clean()
{
    if (m_overlay)
    {
        SDL_DestroyTexture(m_overlay);
        m_overlay = nullptr;
    }
    if (m_pixelFormat)
    {
        SDL_FreeFormat(m_pixelFormat);
        m_pixelFormat = nullptr;
    }
    m_renderer = nullptr;
    m_overlayWidth = 0;
    m_overlayHeight = 0;
    m_overlayFailed = false;
}
