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

AssetManager *Game::assets = new AssetManager(&manager);

auto &player(manager.addEntity());
//auto &thunder(manager.addEntity());
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

    assets->AddTexture("knight", "../assets/link_walk_right.png");
    assets->AddTexture("thunder", "../assets/thunder.png");
    assets->AddTexture("skeleton", "../assets/skeleton.png");
    assets->AddTexture("arrow", "../assets/arrow.png");

    Map::LoadMapXml("../assets/stupid_map_2.xml");

    player.addComponent<TransformComponent>(200, 200, 32, 32, 1);
    player.addComponent<SpriteComponent>("knight", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("knight");
    player.addGroup(groupPlayers);
    auto player_pos = player.getComponent<TransformComponent>();
    /* thunder.addComponent<TransformComponent>(player_pos.position.x + player_pos.width, player_pos.position.y - 16, 64, 64, 2);
    thunder.addComponent<ColliderComponent>("thunder");
    thunder.addComponent<SpriteComponent>("thunder", false);
    thunder.addComponent<KeyboardController>();
 */
    assets->CreateProjectile(Vector2D(220, 220), Vector2D(1, 0), 1000, 1, "arrow");
    assets->CreateProjectile(Vector2D(220, 220), Vector2D(1, 1), 1000, 1, "arrow");

    for (int i = 0; i < NUMBER_OF_ENEMIES; i++)
    {
        auto &enemy = manager.addEntity();
        enemy.addComponent<TransformComponent>(800 - 64, 640 - (i + 1) * 64, 32, 32, 2);
        enemy.addComponent<SpriteComponent>("skeleton", true);
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
#define get_collider(x) getComponent<ColliderComponent>().collider.x
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
    for (auto &p : projectiles)
    {
        for (auto &enemy : enemies)
        {
            if (Collision::AABB(enemy->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
            {
                std::cout << "Hit enemy!" << std::endl;
                p->destroy();
            }
        }
    }
    for (auto &enemy : enemies)
    {
        Vector2D enemy_pos = enemy->getComponent<TransformComponent>().position;
        Vector2D player_pos = player.getComponent<TransformComponent>().position;
        Vector2D towards_player = (player_pos - enemy_pos);
        Vector2D lvelocity = towards_player / towards_player.Length() / 3;
        if (towards_player.x < 0)
        {
            enemy->getComponent<SpriteComponent>().spriteflip = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            enemy->getComponent<SpriteComponent>().spriteflip = SDL_FLIP_NONE;
        }
        enemy->getComponent<TransformComponent>().velocity = lvelocity;
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, enemy->getComponent<ColliderComponent>().collider))
        {
            player.getComponent<TransformComponent>().position = playerPos;
        }
    }
    // TODO: put in fuction:
    for (auto &loc_enemy : enemies)
    {
        for (auto &loc_enemy2 : enemies)
        {
            if (&loc_enemy2 == &loc_enemy)
                continue;
            if (Collision::AABB(loc_enemy->getComponent<ColliderComponent>().collider, loc_enemy2->getComponent<ColliderComponent>().collider))
            {

                int maxX = std::max(loc_enemy->getComponent<ColliderComponent>().collider.x, loc_enemy2->getComponent<ColliderComponent>().collider.x); // Minimum of the boxes' right-side points (top right and bottom right) x coordinates
                int maxY = std::max(loc_enemy->getComponent<ColliderComponent>().collider.y, loc_enemy2->getComponent<ColliderComponent>().collider.y); // Minimum of the boxes' right-side points (top right and bottom right) x coordinates
                int minX = std::min(loc_enemy->get_collider(x) + loc_enemy->get_collider(w), loc_enemy2->get_collider(x) + loc_enemy2->get_collider(w));
                int minY = std::min(loc_enemy->get_collider(y) + loc_enemy->get_collider(h), loc_enemy2->get_collider(y) + loc_enemy2->get_collider(h));
                int distHoriz = minX - maxX; // The horizontal intersection distance
                int distVert = minY - maxY;  // The vertical instersection distance

                // If the boxes are overlapping less on the horizontal axis than the vertical axis,
                // move one of the sprites (in this case, sprite0) in the opposite direction of the
                // x-axis overlap
                if (abs(distHoriz) < abs(distVert))
                {
                    loc_enemy->getComponent<TransformComponent>().velocity.x *= (loc_enemy2->getComponent<TransformComponent>().velocity.x < 0 ? 1 : -1);
                }
                // Else, move one of the sprites (again, I just decided to use sprite0 here,
                // arbitrarily) in the opposite direction of the y-axis overlap
                else
                {
                    loc_enemy->getComponent<TransformComponent>().velocity.y *= (loc_enemy2->getComponent<TransformComponent>().velocity.y < 0 ? 1 : -1);
                }
            }
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
    //thunder.getComponent<TransformComponent>().position = playerPos + Vector2D(64, -16);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned!" << std::endl;
}
