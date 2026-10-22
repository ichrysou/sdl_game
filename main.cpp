
#include <iostream>

#include "game.h"
Game *game = nullptr;

int main(int argc, char *argv[])
{
    game = new Game();
    game->init("THE GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, true);
    const Uint64 performanceFrequency = SDL_GetPerformanceFrequency();
    const Uint64 frameDuration = performanceFrequency / 60;
    while (game->running())
    {
        const Uint64 frameStart = SDL_GetPerformanceCounter();
        game->handleEvents();

        game->update();
        game->render();
        const Uint64 elapsed = SDL_GetPerformanceCounter() - frameStart;
        if (elapsed < frameDuration)
            SDL_Delay(static_cast<Uint32>((frameDuration - elapsed) * 1000 / performanceFrequency));
    }

    game->clean();
    delete game;
    return 0;
}
