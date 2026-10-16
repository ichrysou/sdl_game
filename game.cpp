
#include "game.h"
#include "Collision.h"
#include "Components.h"
#include "Map.h"
#include "Systems.h"
#include "Vector2D.h"
#include "textureManager.h"
#include <algorithm>
#include <cmath>
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
    assets->AddTexture("skeleton-ice", "../assets/skeleton_ice.png");
    assets->AddTexture("skeleton-moss", "../assets/skeleton_moss.png");
    assets->AddTexture("player-azure", "../assets/player_azure.png");
    assets->AddTexture("player-crimson", "../assets/player_crimson.png");
    assets->AddTexture("arrow", "../assets/arrow.png");

    Map::LoadMapXml("../assets/forest_path.xml");
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
        const char *enemyTexture = i % 3 == 1 ? "skeleton-ice" : (i % 3 == 2 ? "skeleton-moss" : "skeleton");
        enemy.addComponent<TransformComponent>(Map::WorldWidth() - 400 + (i % 2) * 80,
                                              80 + (i / 2) * 112, 32, 32, 2);
        enemy.addComponent<SpriteComponent>(enemyTexture);
        enemy.addComponent<AnimationComponent>(enemyTexture);
        enemy.getComponent<AnimationComponent>().addAnimation(Animation("idle", enemyTexture, 10, 30, 0, 32, 32, 32));
        enemy.getComponent<AnimationComponent>().addAnimation(Animation("walk", enemyTexture, 10, 30, 0, 64, 32, 32));
        enemy.getComponent<AnimationComponent>().addAnimation(Animation("die", enemyTexture, 10, 130, 0, 128, 32, 32, false));
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

        case SDL_KEYDOWN:
            if (event.key.repeat == 0 &&
                (event.key.keysym.sym == SDLK_1 || event.key.keysym.sym == SDLK_2)) {
                const char *mapPath = event.key.keysym.sym == SDLK_1
                                          ? "../assets/forest_path.xml"
                                          : "../assets/ruined_courtyard.xml";
                if (Map::LoadMapXml(mapPath)) {
                    auto &transform = player.getComponent<TransformComponent>();
                    transform.position = Vector2D(200, 200);
                    player.getComponent<ColliderComponent>().update();
                    auto &mapEnemies = manager.getGroup(groupEnemies);
                    for (std::size_t i = 0; i < mapEnemies.size(); ++i) {
                        auto &enemyTransform = mapEnemies[i]->getComponent<TransformComponent>();
                        enemyTransform.position = Vector2D(
                            Map::WorldWidth() - 400 + (i % 2) * 80,
                            80 + (i / 2) * 112);
                        mapEnemies[i]->getComponent<ColliderComponent>().update();
                    }
                    camera.x = 0;
                    camera.y = 0;
                }
            }
            if (event.key.repeat == 0 && event.key.keysym.sym >= SDLK_F1 &&
                event.key.keysym.sym <= SDLK_F3) {
                static const char *playerTextures[] = {"knight", "player-azure", "player-crimson"};
                const int selected = event.key.keysym.sym - SDLK_F1;
                player.getComponent<AnimationComponent>().setTextureOverride(playerTextures[selected]);
            }
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
    std::vector<Vector2D> previousEnemyPositions;
    previousEnemyPositions.reserve(enemies.size());
    for (auto &enemy : enemies) {
        previousEnemyPositions.push_back(enemy->getComponent<TransformComponent>().position);
    }
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
        float distance = towards_player.Length();
        Vector2D lvelocity;
        if (distance > 0.0f) {
            lvelocity.x = towards_player.x / distance / 3.0f;
            lvelocity.y = towards_player.y / distance / 3.0f;
        }
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

    for (std::size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i]->hasComponent<DeadComponent>()) continue;
        for (std::size_t j = i + 1; j < enemies.size(); ++j) {
            if (enemies[j]->hasComponent<DeadComponent>()) continue;
            auto &transformA = enemies[i]->getComponent<TransformComponent>();
            auto &transformB = enemies[j]->getComponent<TransformComponent>();
            float dx = transformB.position.x - transformA.position.x;
            float dy = transformB.position.y - transformA.position.y;
            float distance = std::sqrt(dx * dx + dy * dy);
            float minimumDistance = 0.75f * std::min(transformA.width * transformA.scale,
                                                    transformB.width * transformB.scale);
            if (distance >= minimumDistance) continue;

            float directionX = distance > 0.0f ? dx / distance : 1.0f;
            float directionY = distance > 0.0f ? dy / distance : 0.0f;
            float pushDistance = (minimumDistance - distance) / 2.0f;
            transformA.position.x -= directionX * pushDistance;
            transformA.position.y -= directionY * pushDistance;
            transformB.position.x += directionX * pushDistance;
            transformB.position.y += directionY * pushDistance;
        }
    }
    for (std::size_t i = 0; i < enemies.size(); ++i) {
        auto &enemy = enemies[i];
        if (enemy->hasComponent<DeadComponent>()) continue;
        auto &transform = enemy->getComponent<TransformComponent>();
        auto &collider = enemy->getComponent<ColliderComponent>();
        Vector2D movedPosition = transform.position;

        transform.position.x = movedPosition.x;
        transform.position.y = previousEnemyPositions[i].y;
        collider.update();
        bool blockedX = false;
        for (auto &wall : colliders) {
            if (Collision::AABB(collider, wall->getComponent<ColliderComponent>())) {
                blockedX = true;
                break;
            }
        }
        if (blockedX) transform.position.x = previousEnemyPositions[i].x;

        transform.position.y = movedPosition.y;
        collider.update();
        for (auto &wall : colliders) {
            if (Collision::AABB(collider, wall->getComponent<ColliderComponent>())) {
                transform.position.y = previousEnemyPositions[i].y;
                break;
            }
        }
        collider.update();
    }

    camera.x = std::max(0, std::min(
        static_cast<int>(player.getComponent<TransformComponent>().position.x) - SCREEN_WIDTH / 2,
        Map::WorldWidth() - camera.w));
    camera.y = std::max(0, std::min(
        static_cast<int>(player.getComponent<TransformComponent>().position.y) - SCREEN_HEIGHT / 2,
        Map::WorldHeight() - camera.h));
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
