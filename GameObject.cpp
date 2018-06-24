#include "GameObject.h"
#include "textureManager.h"

GameObject::GameObject(const char *texturesheet, int x, int y) {
    objTexture = TextureManager::LoadTexture(texturesheet);
    xpos = x;
    ypos = y;
}

void GameObject::Update() {

    xpos++;
    ypos++;

    srcRect.h = 64;
    srcRect.w = 64;
    srcRect.x = 0;
    srcRect.y = 0;

    dstRect.x = xpos;
    dstRect.y = ypos;
    dstRect.w = srcRect.w;
    dstRect.h = srcRect.h;
}

void GameObject::Render() {
    SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &dstRect);
}
