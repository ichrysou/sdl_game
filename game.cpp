#include "game.h"
#include "textureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"

Map *map;
SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;
Manager manager;

std::vector<ColliderComponent*> Game::colliders;

auto& player(manager.addEntity());
auto& wall(manager.addEntity());

auto& tile0(manager.addEntity());
auto& tile1(manager.addEntity());
auto& tile2(manager.addEntity());

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

    tile0.addComponent<TileComponent>(200, 200, 32, 32, 0);
    tile1.addComponent<TileComponent>(250, 250, 32, 32, 1);

    tile1.addComponent<ColliderComponent>("dirt");

    tile2.addComponent<TileComponent>(150, 150, 32, 32, 2);

    player.addComponent<TransformComponent>();
    player.addComponent<SpriteComponent>("/home/ichrysou/Downloads/knight.png");
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");

    wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 30, 1);
    wall.addComponent<SpriteComponent>("../assets/dirt.png");
    wall.addComponent<ColliderComponent>("wall");
}

void Game::handleEvents() {

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
    manager.draw();
    SDL_RenderPresent(renderer);
}

void Game::update() {
    manager.refresh();
    manager.update();
    if (player.getComponent<TransformComponent>().position.x > 400) {
        player.getComponent<SpriteComponent>().setTex("../assets/enemy.png");
    }
    if (Collision::AABB(player.getComponent<ColliderComponent>().collider
                        , wall.getComponent<ColliderComponent>().collider)) {
        std::cout << "Wall Hit" << std::endl;
        player.getComponent<TransformComponent>().velocity * -1;
    }

}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned!" << std::endl;
}
