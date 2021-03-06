#include "textureManager.h"
#include "game.h"
#include <SDL_image.h>

#include <stdlib.h>
#include <stdio.h>
SDL_Texture *TextureManager::LoadTexture(const char *texture)
{
    SDL_Surface *tempSurface = IMG_Load(texture);
    if (!tempSurface)
    {
        printf("%s\n", IMG_GetError());
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    if (!tex)
    {
        printf("%s\n", SDL_GetError());
    }
    SDL_FreeSurface(tempSurface);

    return tex;
}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, double angle)
{
//    SDL_SetTextureColorMod(tex, 125, 125, 25);
    SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, angle, NULL, flip);
};

void TextureManager::DrawSquare(SDL_Rect rect){
    SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
    rect.x -= Game::camera.x;
    rect.y -= Game::camera.y;
    SDL_RenderDrawRect(Game::renderer, &rect);
}
