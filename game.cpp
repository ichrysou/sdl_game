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

bool Game::isRunning = false;

SDL_Rect Game::camera = {
    0,
    0,
    SCREEN_WIDTH,
    SCREEN_HEIGHT};

auto &player(manager.addEntity());
auto &thunder(manager.addEntity());
//auto& wall(manager.addEntity());

Game::~Game() {}

Game::Game()
{
    cntr = 0;
}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen)
        flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "Subsystems Initialized! ..." << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (window)
        {
            std::cout << "Window created!" << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            std::cout << "Renderer created!" << std::endl;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        isRunning = true;
    }
    else
    {
        isRunning = false;
    }

    //Map::LoadMap("../assets/tilesets/map2", 8, 8);
    Map::LoadMapXml("../assets/stupid_map_2.xml");

    player.addComponent<TransformComponent>(200, 200, 64, 64, 1);
    player.addComponent<SpriteComponent>("knight", "../assets/knight.png", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);
    auto player_pos = player.getComponent<TransformComponent>();
    thunder.addComponent<TransformComponent>(player_pos.position.x + player_pos.width, player_pos.position.y - 16, 64, 64, 2);
    thunder.addComponent<ColliderComponent>("thunder");
    thunder.addComponent<SpriteComponent>("thunder", "../assets/thunder.png", false);

    thunder.addComponent<KeyboardController>();
    thunder.addGroup(groupProjectiles);

    for (int i = 0; i < NUMBER_OF_ENEMIES; i++)
    {
        auto &enemy = manager.addEntity();
        enemy.addComponent<TransformComponent>(800 - 64, 640 - (i + 1) * 64, 32, 32, 2);
        enemy.addComponent<SpriteComponent>("skeleton", "../assets/skeleton.png", true);
        enemy.addComponent<ColliderComponent>("skeleton");
        enemy.addGroup(groupEnemies);
    }
    //wall.addComponent<TransformComponent>(300.0f, 300.0f, 32, 32, 1);
    //wall.addComponent<SpriteComponent>("../assets/dirt.png");
    //wall.addComponent<ColliderComponent>("wall");
}

void Game::handleEvents()
{

    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;

    default:
        break;
    }
}
// this is where we add all our textures to be rendered
auto &tiles(manager.getGroup(Game::groupMap));
auto &players(manager.getGroup(Game::groupPlayers));
auto &enemies(manager.getGroup(Game::groupEnemies));
auto &projectiles(manager.getGroup(Game::groupProjectiles));
auto &colliders(manager.getGroup(Game::groupColliders));
void Game::render()
{
    SDL_RenderClear(renderer);
    // NULL, NULL: use the whole image and render it to the whole rectangle
    // this is where we could add stuff to render
    for (auto &tile : tiles)
    {
        tile->draw();
    }
    for (auto &player : players)
    {
        player->draw();
    }
    for (auto &enemy : enemies)
    {
        enemy->draw();
    }
    for (auto &projectile : projectiles)
    {
        projectile->draw();
    }

    SDL_RenderPresent(renderer);
}

void Game::update()
{
    Vector2D playerPos = player.getComponent<TransformComponent>().position;
    manager.refresh();
    manager.update();

    for (auto &col : colliders)
    {
        if (Collision::AABB(player.getComponent<ColliderComponent>(), col->getComponent<ColliderComponent>()))
        {
            std::cout << "collision" << std::endl;
            player.getComponent<TransformComponent>().position = playerPos;
        }
    }
/* for platformers: */
#ifdef PLATFORMER
    int player_speed = player.getComponent<TransformComponent>().speed;
    Vector2D player_velocity = player.getComponent<TransformComponent>().velocity;
    tile->getComponent<TileComponent>().dstRect.x += -(player_velocity.x * player_speed);
    tile->getComponent<TileComponent>().dstRect.y += -(player_velocity.y * player_speed);
#endif

    for (auto &enemy : enemies)
    {
        float speed_x = -1 + rand() % 3;
        speed_x *= rand() % 1 ? 0 : 1;
        float speed_y = -1 + rand() % 3;
        speed_y *= rand() % 1 ? 0 : 1;
        Vector2D lvelocity(speed_x, speed_y);
        Vector2D enemyPos = enemy->getComponent<TransformComponent>().position;

        if (!(SDL_GetTicks() % 20))
        {
            Vector2D player_pos = player.getComponent<TransformComponent>().position;
            Vector2D towards_player = (player_pos - enemy->getComponent<TransformComponent>().position);
            lvelocity = ((towards_player / towards_player.Length()) * 3);
        }
        enemy->getComponent<TransformComponent>().velocity = lvelocity;
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, enemy->getComponent<ColliderComponent>().collider))
        {
            player.getComponent<TransformComponent>().position = playerPos;
        }
        if (Collision::AABB(thunder.getComponent<ColliderComponent>().collider, enemy->getComponent<ColliderComponent>().collider))
        {
            enemy->getComponent<TransformComponent>().position = enemyPos;
            enemy->getComponent<TransformComponent>().velocity.Zero();
        }
    }

    camera.x = player.getComponent<TransformComponent>().position.x - SCREEN_WIDTH / 2;
    camera.y = player.getComponent<TransformComponent>().position.y - SCREEN_HEIGHT / 2;
    if (camera.x < 0)
    {
        camera.x = 0;
    }
    if (camera.y < 0)
    {
        camera.y = 0;
    }
    if (camera.x > camera.w)
    {
        camera.x = camera.w;
    }
    if (camera.y > camera.h)
    {
        camera.y = camera.h;
    }
    thunder.getComponent<TransformComponent>().position = playerPos + Vector2D(64, -16);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned!" << std::endl;
}
