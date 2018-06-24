#include "game.h"
#include "textureManager.h"
#include "Map.h"
#include "ECS/Components.h"

Map *map;
SDL_Renderer *Game::renderer = nullptr;

Manager manager;

auto& player(manager.addEntity());

Game::~Game() {}

Game::Game() {
    cntr = 0;
}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystems Initialized! ..." << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (window) {
            std::cout << "Window created!" << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            std::cout << "Renderer created!" << std::endl;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        isRunning = true;
    } else {
        isRunning = false;
    }

    map = new Map();
    player.addComponent<PositionComponent>();
    player.addComponent<SpriteComponent>("assets/player.png");
}

void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;

        default:
            break;
    }
}
// this is where we add all our textures to be rendered
void Game::render() {
    SDL_RenderClear(renderer);
    map->DrawMap();
    // NULL, NULL: use the whole image and render it to the whole rectangle
    // this is where we could add stuff to render
    SDL_RenderPresent(renderer);
}

void Game::update() {
    manager.refresh();
    manager.update();

}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned!" << std::endl;
}
