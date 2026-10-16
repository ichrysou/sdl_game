#include "Map.h"
#include "game.h"
#include <fstream>
#include "pugixml.hpp"
#include "Components.h"
#include <iostream>

extern Manager manager;
const char *tilesheet = "../assets/tiles2.png"; // TODO: find a better way to pass this. Problem is the static functions here
namespace {
int mapWorldWidth = 0;
int mapWorldHeight = 0;

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
    int srcX = index % 4 * tilewidth;
    int srcY = index / 4 * tileheight;
    Map::AddTile(srcX, srcY, x * tilewidth, y * tileheight, tilewidth, tileheight, rotation, 2);
}

void addColliderAt(int x, int y, int tilewidth, int tileheight)
{
    auto &collider(manager.addEntity());
    collider.addComponent<ColliderComponent>("terrain", x * tilewidth, y * tileheight,
                                               tilewidth, tileheight, 2);
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
    mapWorldWidth = tileswide * tilewidth * 2;
    mapWorldHeight = tileshigh * tileheight * 2;

    for (pugi::xml_node item : layer0.children())
    {
        if (std::string(item.name()) == "fill")
        {
            int index = item.attribute("index").as_int();
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
            for (int row = y; row < y + height; ++row)
                for (int column = x; column < x + width; ++column)
                    addTileAt(column, row, index, 0, tilewidth, tileheight);
        }
        else if (std::string(item.name()) == "tile")
        {
            addTileAt(item.attribute("x").as_int(), item.attribute("y").as_int(),
                      item.attribute("index").as_int(), item.attribute("rot").as_int(),
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

void Map::AddTile(int srcX, int srcY, int x, int y, int width, int height, int rotation, int scale)
{
    auto &tile(manager.addEntity());
    //TODO: change tile size it can be read form the map file
    tile.addComponent<TileComponent>(srcX, srcY, x, y, width, height, rotation, tilesheet, scale);
    tile.addGroup(Game::groupMap);
}
