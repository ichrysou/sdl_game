#include "Map.h"
#include "game.h"
#include <SDL.h>
#include <fstream>
#include "pugixml.hpp"
#include "Components.h"
#include <iostream>

extern Manager manager;
const char *tilesheet = "../assets/tileset_new.png"; // TODO: find a better way to pass this. Problem is the static functions here
namespace {
constexpr int mapScale = 1;
int mapWorldWidth = 0;
int mapWorldHeight = 0;
std::vector<SDL_Point> torchPositions;

// Deterministic per-tile hash so a map looks the same every load, but
// adjacent tiles of the same index are drawn from different variants /
// orientations. This breaks up the repetition of large uniform tile areas.
unsigned int posHash(int x, int y)
{
    unsigned int h = (unsigned int)(x * 73856093) ^ (unsigned int)(y * 19349663) ^ 0x9e3779b9u;
    h ^= h >> 16;
    h *= 0x85ebca6bu;
    h ^= h >> 13;
    h *= 0xc2b2ae35u;
    h ^= h >> 16;
    return h;
}

// Visual variants for tile types that can repeat across large areas.
// Picking per-tile (deterministically by position) keeps the map stable
// while making floors/walls look less like a single stamped image.
const int stoneFloorVariants[] = {0, 4, 8, 14, 15};
constexpr int stoneFloorVariantCount = sizeof(stoneFloorVariants) / sizeof(stoneFloorVariants[0]);

bool isStoneFloor(int index)
{
    for (int i = 0; i < stoneFloorVariantCount; ++i)
        if (stoneFloorVariants[i] == index)
            return true;
    return false;
}

void clearMapEntities()
{
    for (auto *tile : manager.getGroup(Game::groupMap))
        tile->destroy();
    for (auto *collider : manager.getGroup(Game::groupColliders))
        collider->destroy();
    manager.refresh();
}

void addTileAt(int x, int y, int index, int rotation, int tilewidth, int tileheight)
{
    if (index < 0)
        index = 3;
    if (index == 16)
        torchPositions.push_back({x * tilewidth + tilewidth / 2,
                                  y * tileheight + tileheight / 2});

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    int rotationDegrees = 0;

    if (isStoneFloor(index))
    {
        // Randomize among stone-floor variants and rotate/flip per tile.
        unsigned int h = posHash(x, y);
        index = stoneFloorVariants[h % stoneFloorVariantCount];
        if (h & 1u)
            flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
        if (h & 2u)
            flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);
        rotationDegrees = ((h >> 2) & 3u) * 90;
    }
    else if (rotation != 0)
    {
        rotationDegrees = rotation * 90;
    }

    int srcX = index % 4 * tilewidth;
    int srcY = index / 4 * tileheight;
    Map::AddTile(srcX, srcY, x * tilewidth, y * tileheight, tilewidth, tileheight, rotationDegrees, flip, mapScale);
}

void addColliderAt(int x, int y, int tilewidth, int tileheight)
{
    auto &collider(manager.addEntity());
    collider.addComponent<ColliderComponent>("terrain", x * tilewidth, y * tileheight,
                                               tilewidth, tileheight, mapScale);
    collider.addGroup(Game::groupColliders);
}
}

Map::Map()
{
}

Map::~Map()
{
}

bool Map::LoadMapXml(std::string tilemap_path)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(tilemap_path.c_str());
    if (!result)
    {
        std::cerr << "Failed to load map '" << tilemap_path << "': " << result.description() << std::endl;
        return false;
    }
    pugi::xml_node tilemap = doc.child("tilemap");
    if (!tilemap)
    {
        std::cerr << "Map '" << tilemap_path << "' has no tilemap root." << std::endl;
        return false;
    }
    int tileswide = tilemap.attribute("tileswide").as_int();
    int tileshigh = tilemap.attribute("tileshigh").as_int();
    int tilewidth = doc.child("tilemap").attribute("tilewidth").as_int();
    int tileheight = doc.child("tilemap").attribute("tileheight").as_int();
    if (tileswide <= 0 || tileshigh <= 0 || tilewidth <= 0 || tileheight <= 0)
    {
        std::cerr << "Map '" << tilemap_path << "' has invalid dimensions." << std::endl;
        return false;
    }

    pugi::xml_node layer0 = tilemap.find_child_by_attribute("layer", "number", "0");
    pugi::xml_node layer1 = tilemap.find_child_by_attribute("layer", "number", "1");
    if (!layer0 || !layer1)
    {
        std::cerr << "Map '" << tilemap_path << "' must have layers 0 and 1." << std::endl;
        return false;
    }

    clearMapEntities();
    torchPositions.clear();
    mapWorldWidth = tileswide * tilewidth * mapScale;
    mapWorldHeight = tileshigh * tileheight * mapScale;
    const std::string mapName = tilemap.attribute("name").as_string();
    const bool outdoorMap = mapName == "Forest Path" || mapName == "Ruined Courtyard";

    for (pugi::xml_node item : layer0.children())
    {
        if (std::string(item.name()) == "fill")
        {
            int index = item.attribute("index").as_int();
            if (outdoorMap && index == 2)
                index = 1;
            for (int y = 0; y < tileshigh; ++y)
                for (int x = 0; x < tileswide; ++x)
                    addTileAt(x, y, index, 0, tilewidth, tileheight);
        }
        else if (std::string(item.name()) == "area")
        {
            int x = item.attribute("x").as_int();
            int y = item.attribute("y").as_int();
            int width = item.attribute("width").as_int();
            int height = item.attribute("height").as_int();
            int index = item.attribute("index").as_int();
            if (outdoorMap && index == 2)
                index = 1;
            for (int row = y; row < y + height; ++row)
                for (int column = x; column < x + width; ++column)
                    addTileAt(column, row, index, 0, tilewidth, tileheight);
        }
        else if (std::string(item.name()) == "tile")
        {
            int index = item.attribute("index").as_int();
            if (outdoorMap && index == 2)
                index = 1;
            addTileAt(item.attribute("x").as_int(), item.attribute("y").as_int(),
                      index, item.attribute("rot").as_int(),
                      tilewidth, tileheight);
        }
    }

    for (pugi::xml_node item : layer1.children())
    {
        if (std::string(item.name()) == "area")
        {
            int x = item.attribute("x").as_int();
            int y = item.attribute("y").as_int();
            int width = item.attribute("width").as_int();
            int height = item.attribute("height").as_int();
            for (int row = y; row < y + height; ++row)
                for (int column = x; column < x + width; ++column)
                    addColliderAt(column, row, tilewidth, tileheight);
        }
        else if (std::string(item.name()) == "tile" && item.attribute("index").as_int() != -1)
        {
            addColliderAt(item.attribute("x").as_int(), item.attribute("y").as_int(),
                          tilewidth, tileheight);
        }
    }
    return true;
}

int Map::WorldWidth()
{
    return mapWorldWidth;
}

int Map::WorldHeight()
{
    return mapWorldHeight;
}

const std::vector<SDL_Point> &Map::TorchPositions()
{
    return torchPositions;
}

void Map::AddTile(int srcX, int srcY, int x, int y, int width, int height, int rotation, SDL_RendererFlip flip, int scale)
{
    auto &tile(manager.addEntity());
    //TODO: change tile size it can be read form the map file
    tile.addComponent<TileComponent>(srcX, srcY, x, y, width, height, rotation, flip, tilesheet, scale);
    tile.addGroup(Game::groupMap);
}
