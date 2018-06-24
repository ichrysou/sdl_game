#include "textureManager.h"
#include <SDL/SDL_image.h>

SDL_Texture * TextureManager::LoadTexture(const char *texture) {
    SDL_Surface *tempSurface = IMG_Load(texture);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    if (!tex) {
        printf("%s\n", SDL_GetError());
    }
    SDL_FreeSurface(tempSurface);

     return tex;
}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest) {
    SDL_RenderCopy(Game::renderer, tex, &src, &dest);
};
