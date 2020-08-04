
#include <iostream>

#include "game.h"
#include "Text.h"

Game *game = nullptr;

int main(int argc, char *argv[])
{

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;
    int prev_time = 0;
    int min = 10000;
    int sum = 0;
    int avg = 0;
    int cntr = 0;
    int fps = 0;
    std::string msg;
    game = new Game();
    game->init("THE GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);
    while (game->running())
    {
        frameStart = SDL_GetTicks();
        game->handleEvents();

        game->update();
        game->render();
        fps = 1000 / frameTime;
        sum += fps;
        if (cntr++ == 100) {
          avg = int(sum / 100.0f);
          cntr = 0;
          sum = 0;
        }
        if ((fps < min) && (fps != 0))
          min = fps;

        if (SDL_GetTicks() - prev_time > 100) {

          msg = std::string("Avg. FPS: ") + std::to_string(avg);
          msg += std::string(" Min. FPS: ") + std::to_string(min);
          prev_time = SDL_GetTicks();
        }

        render_text(0, 0, msg, Game::renderer);

        frameTime = SDL_GetTicks() - frameStart;


        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    game->clean();
    return 0;
}
