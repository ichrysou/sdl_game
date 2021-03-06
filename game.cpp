
#include "game.h"
#include "Collision.h"
#include "Components.h"
#include "Map.h"
#include "Systems.h"
#include "Vector2D.h"
#include "textureManager.h"
#include <SDL_ttf.h>

#define NUMBER_OF_ENEMIES 10

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

Manager manager;

bool Game::isRunning = false;

SDL_Rect Game::camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

AssetManager *Game::assets = new AssetManager(&manager);
// create systems
auto movement_sys = PlayerMovementSystem(manager);
auto arrow_sys = ArrowSystem(manager);
auto cleanup_dead_sys = DeadEntitiesCleanupSystem(manager);

auto &player(manager.addEntity());

Game::~Game() {}

Game::Game() { cntr = 0; }

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


    int imgFlags = IMG_INIT_PNG;
    IMG_Init(imgFlags);
    if (TTF_Init() < 0) {
        std::cout << "error" << std::endl;
    }



    assets->AddTexture("knight", "../assets/link.png");
    assets->AddTexture("thunder", "../assets/thunder.png");
    assets->AddTexture("skeleton", "../assets/skeleton.png");
    assets->AddTexture("arrow", "../assets/arrow.png");

    Map::LoadMapXml("../assets/stupid_map_2.xml");
    player.addComponent<AnimationComponent>("player");

    player.getComponent<AnimationComponent>().addAnimation(Animation("bow-down", "knight", 3, 150, 0, 60, 30, 24, false));
    player.getComponent<AnimationComponent>().addAnimation(Animation("idle", "knight", 1, 1, 330, 120, 30, 24));
    player.getComponent<AnimationComponent>().addAnimation(
        Animation("idle-up", "knight", 1, 1, 210, 120, 30, 24));  // TODO: make speed 0 for motionless animations
    player.getComponent<AnimationComponent>().addAnimation(Animation("idle-down", "knight", 1, 1, 120, 30, 30, 24));
    player.getComponent<AnimationComponent>().addAnimation(Animation("bow", "knight", 3, 150, 240, 150, 30, 24, false));
    player.getComponent<AnimationComponent>().addAnimation(Animation("walk", "knight", 3, 30, 240, 120, 30, 24));
    player.getComponent<AnimationComponent>().addAnimation(Animation("walk-up", "knight", 8, 30, 0, 120, 30, 24));
    player.getComponent<AnimationComponent>().addAnimation(Animation("walk-down", "knight", 8, 30, 0, 30, 30, 24));
    // player.getComponent<AnimationComponent>().addAnimation(Animation("walk-vertical", "knight", 1, 2, 10, 0, 0, 32, 32));
    player.addComponent<TransformComponent>(200, 200, 25, 20, 2);
    player.addComponent<SpriteComponent>("knight");
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("knight");
    player.addGroup(groupPlayers);
    auto player_pos = player.getComponent<TransformComponent>();

    // assets->CreateProjectile(Vector2D(220, 220), Vector2D(0, 1), 1000, 1, "arrow", 90);
    // assets->CreateProjecptile(Vector2D(220, 220), Vector2D(0, 1), 1000, 1, "arrow", Vector2D(0, 1).getAngle());

    for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
        auto &enemy = manager.addEntity();
        enemy.addComponent<TransformComponent>(800 - 64, 640 - (i + 1) * 64, 32, 32, 2);
        enemy.addComponent<SpriteComponent>("skeleton");
        enemy.addComponent<AnimationComponent>("skeleton");
        enemy.getComponent<AnimationComponent>().addAnimation(Animation("idle", "skeleton", 10, 30, 0, 32, 32, 32));
        enemy.getComponent<AnimationComponent>().addAnimation(Animation("walk", "skeleton", 10, 30, 0, 64, 32, 32));
        enemy.getComponent<AnimationComponent>().addAnimation(Animation("die", "skeleton", 10, 130, 0, 128, 32, 32, false));
        enemy.getComponent<AnimationComponent>().setActive("walk");
        enemy.addComponent<ColliderComponent>("skeleton");
        enemy.addGroup(groupEnemies);
    }
}

void Game::handleEvents() {
    if (!SDL_PollEvent(&event)) {
        event.type = 0;
    }

    switch (event.type) {
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
void Game::render() {
    SDL_RenderClear(renderer);
    // NULL, NULL: use the whole image and render it to the whole rectangle
    // this is where we could add stuff to render
    // manager.draw();
    for (auto &tile : tiles) {
        tile->draw();
    }
    for (auto &enemy : enemies) {
      enemy->draw();
    }
    for (auto &player : players) {
      player->draw();
    }
    for (auto &projectile : projectiles) {
        projectile->draw();
    }
    for (auto &collider : colliders) {
        collider->draw();
    }

//    SDL_RenderPresent(renderer);
}
#define get_collider(x) getComponent<ColliderComponent>().collider.x
void Game::update() {
    Vector2D playerPos = player.getComponent<TransformComponent>().position;
    manager.refresh();
    manager.update();
    for (auto &col : colliders) {
        if (Collision::AABB(player.getComponent<ColliderComponent>(), col->getComponent<ColliderComponent>())) {
            player.getComponent<TransformComponent>().position = playerPos;
        }
    }

    for (auto &enemy : enemies) {
        if (enemy->hasComponent<DeadComponent>()) continue;
        Vector2D enemy_pos = enemy->getComponent<TransformComponent>().position;
        Vector2D player_pos = player.getComponent<TransformComponent>().position;
        Vector2D towards_player = (player_pos - enemy_pos);
        Vector2D lvelocity = towards_player / towards_player.Length() / 3;
        if (towards_player.x < 0) {
            enemy->getComponent<SpriteComponent>().spriteflip = SDL_FLIP_HORIZONTAL;
        } else {
            enemy->getComponent<SpriteComponent>().spriteflip = SDL_FLIP_NONE;
        }
        enemy->getComponent<TransformComponent>().velocity = lvelocity;
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, enemy->getComponent<ColliderComponent>().collider)) {
            player.getComponent<TransformComponent>().position = playerPos;
        }
    }

    camera.x = player.getComponent<TransformComponent>().position.x - SCREEN_WIDTH / 2;
    camera.y = player.getComponent<TransformComponent>().position.y - SCREEN_HEIGHT / 2;
    if (camera.x < 0) {
        camera.x = 0;
    }
    if (camera.y < 0) {
        camera.y = 0;
    }
    if (camera.x > camera.w) {
        camera.x = camera.w;
    }
    if (camera.y > camera.h) {
        camera.y = camera.h;
    }
    // thunder.getComponent<TransformComponent>().position = playerPos + Vector2D(64, -16);
    // TODO: add systems to System Manager and update in the game engine just like components

    movement_sys.update();
    arrow_sys.update();
    cleanup_dead_sys.update();
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned!" << std::endl;
}
