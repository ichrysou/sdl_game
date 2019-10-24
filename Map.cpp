#include "Map.h"
#include "game.h"
#include <fstream>
#include "pugixml.hpp"
#include "Components.h"

extern Manager manager;
const char *tilesheet = "../assets/tiles2.png"; // TODO: find a better way to pass this. Problem is the static functions here
Map::Map()
{
}

Map::~Map()
{
}

void Map::LoadMapXml(std::string tilemap_path)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(tilemap_path.c_str());
    if (!result)
        return;
    int tileswide = doc.child("tilemap").attribute("tileswide").as_int();
    int tileshigh = doc.child("tilemap").attribute("tileshigh").as_int();
    int tilewidth = doc.child("tilemap").attribute("tilewidth").as_int();
    int tileheight = doc.child("tilemap").attribute("tileheight").as_int();
    pugi::xpath_node_set layer0 = doc.select_nodes("/tilemap/layer[@number='0']");

    for (pugi::xpath_node tile : layer0[0].node().children())
    {
        pugi::xml_node node = tile.node();
        int x = node.attribute("x").as_int();
        int y = node.attribute("y").as_int();
        int index = node.attribute("index").as_int();
        int rot = node.attribute("rot").as_int();
        if (index < 0)
            index = 3;                    // TODO: decide what happens
        int srcX = index % 4 * tilewidth; //TODO: load metadata for source png here as well to derive the
        int srcY = index / 4 * tileheight;
        int scale = 2;
        AddTile(srcX, srcY, x * tilewidth, y * tileheight, tilewidth, tileheight, rot, scale); //TODO: remove some params
    }
    pugi::xpath_node_set layer1 = doc.select_nodes("/tilemap/layer[@number='1']"); // colliders

    for (pugi::xpath_node tile : layer1[0].node().children())
    {

        pugi::xml_node node = tile.node();
        int x = node.attribute("x").as_int();
        int y = node.attribute("y").as_int();

        int index = node.attribute("index").as_int();
        int rot = node.attribute("rot").as_int();
        if (index != -1)
        {

            auto &col(manager.addEntity());
            std::cout << "x " << x * tilewidth << "y " << y * tileheight <<" index " << index<<std::endl;
            col.addComponent<ColliderComponent>("terrain", x * tilewidth, y * tileheight, tilewidth, tileheight, 2); //TODO: rmv magic number for scale
            col.addGroup(Game::groupColliders);
        }
    }
}

void Map::AddTile(int srcX, int srcY, int x, int y, int width, int height, int rotation, int scale)
{
    auto &tile(manager.addEntity());
    //TODO: change tile size it can be read form the map file
    tile.addComponent<TileComponent>(srcX, srcY, x, y, width, height, rotation, tilesheet, scale);
    tile.addGroup(Game::groupMap);
}
