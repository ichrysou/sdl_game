#pragma once
#include <SDL.h>


class TextureManager
{
  public:
    static SDL_Texture *LoadTexture(const char *fileName);
    static void Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, double angle=NULL);
    static void DrawSquare(SDL_Rect rect);
};
