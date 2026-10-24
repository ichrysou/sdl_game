
#include "game.h"
#include "Collision.h"
#include "Components.h"
#include "Map.h"
#include "Systems.h"
#include "Vector2D.h"
#include "textureManager.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <random>
#include <SDL_ttf.h>

#define NUMBER_OF_ENEMIES 16

extern Manager manager;
extern Entity &player;

namespace {
std::mt19937 randomEngine(std::random_device{}());
constexpr std::size_t MAX_ACTIVE_ENEMIES = 24;
constexpr Uint32 MIN_ENEMY_SPAWN_INTERVAL_MS = 2500;
constexpr Uint32 MAX_ENEMY_SPAWN_INTERVAL_MS = 5000;
Uint32 nextEnemySpawnAt = 0;

bool overlapsAny(const SDL_Rect &candidate, const std::vector<Entity *> &entities)
{
    for (const auto *entity : entities) {
        if (entity->hasComponent<ColliderComponent>() &&
            Collision::AABB(candidate, entity->getComponent<ColliderComponent>().collider))
            return true;
    }
    return false;
}

bool placeHeroAtNearestClearPosition(const Vector2D &preferredPosition)
{
    auto &transform = player.getComponent<TransformComponent>();
    auto &collider = player.getComponent<ColliderComponent>();
    collider.update();

    const int worldWidth = Map::WorldWidth();
    const int worldHeight = Map::WorldHeight();
    const int maximumX = worldWidth - collider.collider.w;
    const int maximumY = worldHeight - collider.collider.h;
    if (maximumX < 0 || maximumY < 0) {
        std::cerr << "Cannot place hero: map is smaller than the hero collider." << std::endl;
        return false;
    }

    const auto &walls = manager.getGroup(Game::groupColliders);
    constexpr int searchStep = 8;
    long long bestDistanceSquared = std::numeric_limits<long long>::max();
    SDL_Rect bestPosition{};
    bool foundPosition = false;

    for (int y = 0; y <= maximumY; y += searchStep) {
        for (int x = 0; x <= maximumX; x += searchStep) {
            SDL_Rect candidate{x, y, collider.collider.w, collider.collider.h};
            if (overlapsAny(candidate, walls))
                continue;

            const long long dx = x - static_cast<int>(preferredPosition.x);
            const long long dy = y - static_cast<int>(preferredPosition.y);
            const long long distanceSquared = dx * dx + dy * dy;
            if (distanceSquared < bestDistanceSquared) {
                bestDistanceSquared = distanceSquared;
                bestPosition = candidate;
                foundPosition = true;
            }
        }
    }

    if (!foundPosition) {
        std::cerr << "Cannot place hero: no collision-free position exists on the map." << std::endl;
        return false;
    }

    transform.position = Vector2D(static_cast<float>(bestPosition.x),
                                  static_cast<float>(bestPosition.y));
    collider.update();
    return true;
}

void placeEnemiesRandomly()
{
    auto &enemies = manager.getGroup(Game::groupEnemies);
    auto &walls = manager.getGroup(Game::groupColliders);
    std::vector<SDL_Rect> reserved;
    reserved.reserve(enemies.size());

    const int enemySize = 32;
    const int worldWidth = Map::WorldWidth();
    const int worldHeight = Map::WorldHeight();
    if (worldWidth <= enemySize || worldHeight <= enemySize) {
        std::cerr << "Cannot place enemies: map is smaller than an enemy." << std::endl;
        return;
    }

    std::uniform_int_distribution<int> xDistribution(0, worldWidth - enemySize);
    std::uniform_int_distribution<int> yDistribution(0, worldHeight - enemySize);
    const auto &playerCollider = player.getComponent<ColliderComponent>().collider;

    for (auto *enemy : enemies) {
        bool placed = false;
        for (int attempt = 0; attempt < 10000; ++attempt) {
            SDL_Rect candidate{xDistribution(randomEngine), yDistribution(randomEngine),
                               enemySize, enemySize};
            if (Collision::AABB(candidate, playerCollider) ||
                overlapsAny(candidate, walls) ||
                std::any_of(reserved.begin(), reserved.end(), [&candidate](const SDL_Rect &other) {
                    return Collision::AABB(candidate, other);
                }))
                continue;

            reserved.push_back(candidate);
            auto &transform = enemy->getComponent<TransformComponent>();
            transform.position = Vector2D(candidate.x, candidate.y);
            enemy->getComponent<ColliderComponent>().update();
            placed = true;
            break;
        }
        if (!placed)
            std::cerr << "Could not find a clear random spawn for an enemy." << std::endl;
    }
}

Entity &addEnemy(int type, int x, int y)
{
    const bool isBlob = type == 0;
    const char *enemyTexture = isBlob ? "blob" : (type == 1 ? "skeleton-ice" : "skeleton-moss");
    auto &enemy = manager.addEntity();
    enemy.addComponent<TransformComponent>(x, y, 32, 32, 1);
    enemy.addComponent<SpriteComponent>(enemyTexture);
    if (isBlob)
        enemy.getComponent<SpriteComponent>().setRenderScale(1.5f);
    enemy.addComponent<AnimationComponent>(enemyTexture);
    if (isBlob) {
        enemy.getComponent<AnimationComponent>().addAnimation(Animation("idle", enemyTexture, 4, 120, 0, 0, 64, 64));
        enemy.getComponent<AnimationComponent>().addAnimation(Animation("walk", enemyTexture, 4, 90, 0, 0, 64, 64));
        enemy.getComponent<AnimationComponent>().addAnimation(Animation("die", enemyTexture, 4, 75, 0, 0, 64, 64, false));
    } else {
        enemy.getComponent<AnimationComponent>().addAnimation(Animation("idle", enemyTexture, 10, 30, 0, 32, 32, 32));
        enemy.getComponent<AnimationComponent>().addAnimation(Animation("walk", enemyTexture, 10, 30, 0, 64, 32, 32));
        enemy.getComponent<AnimationComponent>().addAnimation(Animation("die", enemyTexture, 10, 130, 0, 128, 32, 32, false));
    }
    enemy.getComponent<AnimationComponent>().setActive("walk");
    enemy.addComponent<ColliderComponent>(isBlob ? "blob" : "skeleton");
    enemy.addGroup(Game::groupEnemies);
    return enemy;
}

bool spawnRandomEnemy()
{
    constexpr int enemySize = 32;
    constexpr float minimumPlayerDistance = 240.0f;
    const int worldWidth = Map::WorldWidth();
    const int worldHeight = Map::WorldHeight();
    if (worldWidth <= enemySize || worldHeight <= enemySize)
        return false;

    std::uniform_int_distribution<int> xDistribution(0, worldWidth - enemySize);
    std::uniform_int_distribution<int> yDistribution(0, worldHeight - enemySize);
    std::uniform_int_distribution<int> typeDistribution(0, 2);
    const auto &playerPosition = player.getComponent<TransformComponent>().position;
    const auto &walls = manager.getGroup(Game::groupColliders);
    const auto &enemies = manager.getGroup(Game::groupEnemies);

    for (int attempt = 0; attempt < 10000; ++attempt) {
        const int x = xDistribution(randomEngine);
        const int y = yDistribution(randomEngine);
        SDL_Rect candidate{x, y, enemySize, enemySize};
        const float dx = x - playerPosition.x;
        const float dy = y - playerPosition.y;
        if (dx * dx + dy * dy < minimumPlayerDistance * minimumPlayerDistance ||
            overlapsAny(candidate, walls) || overlapsAny(candidate, enemies))
            continue;

        addEnemy(typeDistribution(randomEngine), x, y);
        return true;
    }
    return false;
}

void scheduleNextEnemySpawn()
{
    std::uniform_int_distribution<Uint32> intervalDistribution(
        MIN_ENEMY_SPAWN_INTERVAL_MS, MAX_ENEMY_SPAWN_INTERVAL_MS);
    nextEnemySpawnAt = SDL_GetTicks() + intervalDistribution(randomEngine);
}

void replenishEnemies()
{
    const Uint32 now = SDL_GetTicks();
    if (!SDL_TICKS_PASSED(now, nextEnemySpawnAt))
        return;

    std::size_t livingEnemies = 0;
    for (const auto *enemy : manager.getGroup(Game::groupEnemies)) {
        if (!enemy->hasComponent<DeadComponent>())
            ++livingEnemies;
    }

    if (livingEnemies < MAX_ACTIVE_ENEMIES)
        spawnRandomEnemy();
    scheduleNextEnemySpawn();
}
}

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

Entity &player(manager.addEntity());

Game::~Game() {}

Game::Game() { cntr = 0; }

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) flags = SDL_WINDOW_FULLSCREEN_DESKTOP;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystems Initialized! ..." << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (!window) {
            std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
            isRunning = false;
            return;
        }
        std::cout << "Window created!" << std::endl;

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer)
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer)
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (renderer) {
            std::cout << "Renderer created!" << std::endl;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            int outputWidth = SCREEN_WIDTH;
            int outputHeight = SCREEN_HEIGHT;
            SDL_GetRendererOutputSize(renderer, &outputWidth, &outputHeight);
            const int logicalWidth = fullscreen ? 1600 : SCREEN_WIDTH;
            const int logicalHeight = fullscreen && outputWidth > 0
                                          ? logicalWidth * outputHeight / outputWidth
                                          : SCREEN_HEIGHT;
            if (SDL_RenderSetLogicalSize(renderer, logicalWidth, logicalHeight) != 0) {
                std::cerr << "Logical render size setup failed: " << SDL_GetError() << std::endl;
            } else {
                camera.w = logicalWidth;
                camera.h = logicalHeight;
            }
        } else {
            std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            window = nullptr;
            isRunning = false;
            return;
        }
        isRunning = true;
    } else {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
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
    assets->AddTexture("hero-warden", "../assets/hero_warden_v2.png");
    assets->AddTexture("hero-cast", "../assets/hero_cast.png");
    assets->AddTexture("arrow", "../assets/arrow.png");
    assets->AddTexture("necro", "../assets/necromancer.png");
    assets->AddTexture("blob", "../assets/blob.png");

    Map::LoadMapXml("../assets/dungeon_crypt.xml");
    lighting.initialize(renderer);
    setupLightingForCurrentMap();
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
    player.getComponent<AnimationComponent>().addAnimation(
        Animation("thunder-right", "hero-cast", 5, 84, 0, 0, 30, 24, false));
    player.getComponent<AnimationComponent>().addAnimation(
        Animation("thunder-down", "hero-cast", 5, 84, 0, 24, 30, 24, false));
    player.getComponent<AnimationComponent>().addAnimation(
        Animation("thunder-up", "hero-cast", 5, 84, 0, 48, 30, 24, false));
    // player.getComponent<AnimationComponent>().addAnimation(Animation("walk-vertical", "knight", 1, 2, 10, 0, 0, 32, 32));
    player.addComponent<TransformComponent>(200, 200, 25, 20, 1);
    player.addComponent<SpriteComponent>("knight");
    // outline removed per feedback
    player.getComponent<AnimationComponent>().setTextureOverride("hero-warden");
    player.getComponent<AnimationComponent>().setActive("idle-down");
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("knight");
    player.addGroup(groupPlayers);
    if (!placeHeroAtNearestClearPosition(Vector2D(200, 200))) {
        isRunning = false;
        return;
    }

    // assets->CreateProjectile(Vector2D(220, 220), Vector2D(0, 1), 1000, 1, "arrow", 90);
    // assets->CreateProjecptile(Vector2D(220, 220), Vector2D(0, 1), 1000, 1, "arrow", Vector2D(0, 1).getAngle());

    for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
        const int x = Map::WorldWidth() - 400 + (i % 2) * 80;
        const int y = 80 + (i / 2) * 112;
        addEnemy(i % 3, x, y);
    }
    placeEnemiesRandomly();
    scheduleNextEnemySpawn();
}

// Spawns the crypt boss in the boss room of assets/dungeon_crypt.xml.
// The room spans tiles (24..45, 46..53); the boss is centered on tile ~34.
static void spawnCryptBoss() {
    static const Vector2D bossSpawn(1120, 1568);
    // Reuse the boss if she is still alive from an earlier visit.
    for (auto *enemy : manager.getGroup(Game::groupEnemies)) {
        if (enemy->hasComponent<ColliderComponent>() &&
            enemy->getComponent<ColliderComponent>().tag == "boss" &&
            !enemy->hasComponent<DeadComponent>()) {
            enemy->getComponent<TransformComponent>().position = bossSpawn;
            enemy->getComponent<ColliderComponent>().update();
            return;
        }
    }

    auto &boss = manager.addEntity();
    boss.addComponent<TransformComponent>(bossSpawn.x, bossSpawn.y, 32, 32, 2); // 64x64, twice the minions
    boss.addComponent<SpriteComponent>("necro");
    boss.addComponent<AnimationComponent>("necro");
    boss.getComponent<AnimationComponent>().addAnimation(Animation("idle", "necro", 2, 250, 0, 0, 32, 32));
    boss.getComponent<AnimationComponent>().addAnimation(Animation("walk", "necro", 4, 110, 0, 32, 32, 32));
    // the "die" animation is what lets DeadEntitiesCleanupSystem remove her
    boss.getComponent<AnimationComponent>().addAnimation(Animation("die", "necro", 4, 110, 0, 64, 32, 32, false));
    // reserved for the future raise-skeletons skill
    boss.getComponent<AnimationComponent>().addAnimation(Animation("revive", "necro", 4, 150, 0, 96, 32, 32, false));
    boss.getComponent<AnimationComponent>().setActive("walk");
    boss.addComponent<ColliderComponent>("boss");
    boss.addGroup(Game::groupEnemies);
}

void Game::handleEvents() {
    while (SDL_PollEvent(&event)) {
        player.getComponent<KeyboardController>().handleEvent(event);
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.repeat == 0 &&
                    event.key.keysym.sym >= SDLK_1 && event.key.keysym.sym <= SDLK_3) {
                    static const char *mapPaths[] = {
                        "../assets/forest_path.xml",
                        "../assets/ruined_courtyard.xml",
                        "../assets/dungeon_crypt.xml"};
                    const char *mapPath = mapPaths[event.key.keysym.sym - SDLK_1];
                    if (Map::LoadMapXml(mapPath)) {
                        setupLightingForCurrentMap();
                        if (placeHeroAtNearestClearPosition(Vector2D(200, 200))) {
                            placeEnemiesRandomly();
                            if (std::string(mapPath) == "../assets/dungeon_crypt.xml")
                                spawnCryptBoss();
                            camera.x = 0;
                            camera.y = 0;
                        }
                    }
                }
                if (event.key.repeat == 0 && event.key.keysym.sym >= SDLK_F1 &&
                    event.key.keysym.sym <= SDLK_F4) {
                    static const char *playerTextures[] = {
                        "knight", "player-azure", "player-crimson", "hero-warden"};
                    const int selected = event.key.keysym.sym - SDLK_F1;
                    player.getComponent<AnimationComponent>().setTextureOverride(playerTextures[selected]);
                }
                if (event.key.repeat == 0 && event.key.keysym.sym == SDLK_F9) {
                    int w, h;
                    SDL_GetRendererOutputSize(renderer, &w, &h);
                    SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA8888);
                    if (surf && SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888,
                                                       surf->pixels, surf->pitch) == 0) {
                        SDL_SaveBMP(surf, "/tmp/opencode/screenshot.bmp");
                        std::cerr << "Screenshot saved to /tmp/opencode/screenshot.bmp" << std::endl;
                    }
                    if (surf) SDL_FreeSurface(surf);
                }
                break;

            default:
                break;
        }
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


    for (auto *enemy : enemies) {
        if (enemy->hasComponent<ColliderComponent>() &&
            enemy->getComponent<ColliderComponent>().tag == "boss" &&
            !enemy->hasComponent<DeadComponent>()) {
            auto &bossTransform = enemy->getComponent<TransformComponent>();
            auto &bossCollider = enemy->getComponent<ColliderComponent>();
            const int barW = bossCollider.collider.w;
            const int barH = 5;
            SDL_Rect barBack = {
                static_cast<int>(bossTransform.position.x - Game::camera.x),
                static_cast<int>(bossTransform.position.y - Game::camera.y) - barH - 3,
                barW, barH
            };
            SDL_SetRenderDrawColor(Game::renderer, 60, 60, 60, 255);
            SDL_RenderFillRect(Game::renderer, &barBack);
            SDL_SetRenderDrawColor(Game::renderer, 210, 60, 60, 255);
            SDL_RenderFillRect(Game::renderer, &barBack);
            SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(Game::renderer, &barBack);
        }
    }

    lighting.render(camera);
    SDL_RenderPresent(renderer);
}
#define get_collider(x) getComponent<ColliderComponent>().collider.x
void Game::update() {
    auto &playerTransform = player.getComponent<TransformComponent>();
    auto &playerCollider = player.getComponent<ColliderComponent>();
    const Vector2D previousPlayerPosition = playerTransform.position;
    auto &playerSprite = player.getComponent<SpriteComponent>();
    manager.refresh();
    replenishEnemies();
    std::vector<Vector2D> previousEnemyPositions;
    previousEnemyPositions.reserve(enemies.size());
    for (auto &enemy : enemies) {
        previousEnemyPositions.push_back(enemy->getComponent<TransformComponent>().position);
    }
    manager.update();
    const Vector2D desiredPlayerPosition = playerTransform.position;
    const auto playerBlocked = [&]() {
        for (auto *wall : colliders) {
            if (Collision::AABB(playerCollider, wall->getComponent<ColliderComponent>()))
                return true;
        }
        for (auto *enemy : enemies) {
            if (!enemy->hasComponent<DeadComponent>() &&
                Collision::AABB(playerCollider, enemy->getComponent<ColliderComponent>())) {
                playerSprite.startFlicker();
                return true;
            }
        }
        return false;
    };

    playerTransform.position.x = desiredPlayerPosition.x;
    playerTransform.position.y = previousPlayerPosition.y;
    playerCollider.update();
    if (playerBlocked())
        playerTransform.position.x = previousPlayerPosition.x;

    playerTransform.position.y = desiredPlayerPosition.y;
    playerCollider.update();
    if (playerBlocked())
        playerTransform.position.y = previousPlayerPosition.y;
    playerCollider.update();

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
    }

    for (std::size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i]->hasComponent<DeadComponent>()) continue;
        for (std::size_t j = i + 1; j < enemies.size(); ++j) {
            if (enemies[j]->hasComponent<DeadComponent>()) continue;
            auto &transformA = enemies[i]->getComponent<TransformComponent>();
            auto &transformB = enemies[j]->getComponent<TransformComponent>();
            auto &colliderA = enemies[i]->getComponent<ColliderComponent>();
            auto &colliderB = enemies[j]->getComponent<ColliderComponent>();
            if (!Collision::AABB(colliderA, colliderB)) continue;

            const int overlapX = std::min(colliderA.collider.x + colliderA.collider.w,
                                          colliderB.collider.x + colliderB.collider.w) -
                                 std::max(colliderA.collider.x, colliderB.collider.x);
            const int overlapY = std::min(colliderA.collider.y + colliderA.collider.h,
                                          colliderB.collider.y + colliderB.collider.h) -
                                 std::max(colliderA.collider.y, colliderB.collider.y);
            if (overlapX <= 0 || overlapY <= 0) continue;

            if (overlapX < overlapY) {
                const float direction = transformA.position.x < transformB.position.x ? -1.0f : 1.0f;
                transformA.position.x += direction * overlapX / 2.0f;
                transformB.position.x -= direction * overlapX / 2.0f;
            } else {
                const float direction = transformA.position.y < transformB.position.y ? -1.0f : 1.0f;
                transformA.position.y += direction * overlapY / 2.0f;
                transformB.position.y -= direction * overlapY / 2.0f;
            }
            colliderA.update();
            colliderB.update();
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
        else if (Collision::AABB(collider, playerCollider)) {
            playerSprite.startFlicker();
            transform.position.x = previousEnemyPositions[i].x;
        }

        transform.position.y = movedPosition.y;
        collider.update();
        if (Collision::AABB(collider, playerCollider)) {
            playerSprite.startFlicker();
            transform.position.y = previousEnemyPositions[i].y;
            collider.update();
        }
        for (auto &wall : colliders) {
            if (Collision::AABB(collider, wall->getComponent<ColliderComponent>())) {
                transform.position.y = previousEnemyPositions[i].y;
                break;
            }
        }
        collider.update();
    }

    camera.x = std::max(0, std::min(
        static_cast<int>(player.getComponent<TransformComponent>().position.x) - camera.w / 2,
        Map::WorldWidth() - camera.w));
    camera.y = std::max(0, std::min(
        static_cast<int>(player.getComponent<TransformComponent>().position.y) - camera.h / 2,
        Map::WorldHeight() - camera.h));
    lighting.clearLights();
    const auto &playerPosition = playerTransform.position;
    lighting.addLight(playerPosition.x + playerTransform.width * playerTransform.scale * 0.5f,
                      playerPosition.y + playerTransform.height * playerTransform.scale * 0.5f,
                      144.0f, 0.82f);
    if (playerSprite.isThunderCasting()) {
        const Vector2D staffTip = playerSprite.getThunderStaffTipWorld(
            playerSprite.getThunderCastProgress());
        const float flash = playerSprite.getThunderCastProgress();
        const float intensity = flash > 0.42f && flash < 0.68f ? 1.0f : 0.9f;
        lighting.addLight(staffTip.x, staffTip.y, 280.0f, intensity);
    }
    for (auto *projectile : projectiles) {
        if (!projectile->isActive())
            continue;

        if (projectile->hasComponent<ThunderMeleeComponent>()) {
            const Vector2D boltTip =
                projectile->getComponent<ThunderMeleeComponent>().getLightPosition();
            lighting.addLight(boltTip.x, boltTip.y, 208.0f, 1.0f);
        }
        if (projectile->hasComponent<BubbleProjectileComponent>()) {
            const auto &bubble =
                projectile->getComponent<BubbleProjectileComponent>();
            const Vector2D lightPosition = bubble.getLightPosition();
            lighting.addLight(lightPosition.x, lightPosition.y,
                              bubble.getLightRadius(), bubble.getLightIntensity());
        }
    }
    const Uint32 flicker = SDL_GetTicks() / 180;
    for (const SDL_Point &torch : Map::TorchPositions()) {
        const float radius = 232.0f + static_cast<float>((torch.x / 13 + torch.y / 7 + flicker) % 5) * 3.0f;
        lighting.addLight(static_cast<float>(torch.x), static_cast<float>(torch.y),
                          radius, 1.0f);
    }
    lighting.update();
    // thunder.getComponent<TransformComponent>().position = playerPos + Vector2D(64, -16);
    // TODO: add systems to System Manager and update in the game engine just like components

    movement_sys.update();
    arrow_sys.update();
    cleanup_dead_sys.update();
}

void Game::clean() {
    lighting.clean();
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    std::cout << "Game Cleaned!" << std::endl;
}

void Game::setupLightingForCurrentMap()
{
    std::vector<SDL_Rect> occluders;
    const auto &walls = manager.getGroup(groupColliders);
    occluders.reserve(walls.size());
    for (const auto *wall : walls)
        occluders.push_back(wall->getComponent<ColliderComponent>().collider);

    lighting.configureMap(Map::WorldWidth(), Map::WorldHeight(), 32, occluders);
    lighting.clearLights();
    for (const SDL_Point &torch : Map::TorchPositions())
        lighting.addLight(static_cast<float>(torch.x), static_cast<float>(torch.y),
                          232.0f, 1.0f);
}
