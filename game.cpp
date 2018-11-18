#include "game.h"
#include "textureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"

#define NUMBER_OF_ENEMIES 10

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

Manager manager;

std::vector<ColliderComponent*> Game::colliders;

auto& player(manager.addEntity());
//auto& wall(manager.addEntity());

enum groupLabels : std::size_t {
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders
};

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

    Map::LoadMap("../assets/tilesets/map", 32, 32);

    player.addComponent<TransformComponent>(200, 200, 64, 64, 1);
    player.addComponent<SpriteComponent>("knight", "../assets/knight.png", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);

    for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
        auto& enemy = manager.addEntity();
        enemy.addComponent<TransformComponent>(600 + i * 32, 800, 32, 32, 2);
        enemy.addComponent<SpriteComponent>("skeleton", "../assets/skeleton.png", true);
        enemy.addComponent<ColliderComponent>("skeleton");
        enemy.addGroup(groupEnemies);
    }
    //wall.addComponent<TransformComponent>(300.0f, 300.0f, 32, 32, 1);
    //wall.addComponent<SpriteComponent>("../assets/dirt.png");
    //wall.addComponent<ColliderComponent>("wall");
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
auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

void Game::render() {
    SDL_RenderClear(renderer);
    // NULL, NULL: use the whole image and render it to the whole rectangle
    // this is where we could add stuff to render
    for (auto &tile : tiles) {
        tile->draw();
    }
    for (auto &player : players) {
        player->draw();
    }
    for (auto &enemy : enemies) {
        enemy->draw();
    }


    SDL_RenderPresent(renderer);
}

void Game::update() {
    Vector2D playerPos = player.getComponent<TransformComponent>().position;
     
    manager.refresh();
    manager.update();
    for (auto& tile : tiles) {
        if (!tile->hasComponent<ColliderComponent>())
            continue;
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider
                        , tile->getComponent<ColliderComponent>().collider)) {
             player.getComponent<TransformComponent>().position = playerPos;
        } 
    }

    for (auto& enemy : enemies) {
        float speed_x = -1 + rand() % 3;
        speed_x *= rand()%1?0:1;
        float speed_y = -1 + rand() % 3;
        speed_y *= rand()%1?0:1;
        Vector2D lvelocity(speed_x, speed_y);
        if (!(SDL_GetTicks() % 20)) {
        Vector2D player_pos = player.getComponent<TransformComponent>().position;
            Vector2D towards_player = (player_pos - enemy->getComponent<TransformComponent>().position);
            lvelocity = ((towards_player / towards_player.Length())*3);  
        }
        enemy->getComponent<TransformComponent>().velocity = lvelocity;
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider
                        , enemy->getComponent<ColliderComponent>().collider)) {
             player.getComponent<TransformComponent>().position = playerPos;
        }
    }

}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned!" << std::endl;
}

void Game::AddTile(int id, int x, int y) {
    auto& tile(manager.addEntity());
    //TODO: change tile size it can be read form the map file
    tile.addComponent<TileComponent>(x, y, 16, 16, id);
    tile.addGroup(groupMap);
    if (id == 1)
        tile.addComponent<ColliderComponent>("wall");
}

