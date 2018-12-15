#include "Map.h"
#include "game.h"
#include <fstream>
#include "pugixml.hpp"

Map::Map() {

}

Map::~Map() {
}

void Map::LoadMap(std::string path, int sizeX, int sizeY) {
    char tile;
    std::fstream mapFile;
    mapFile.open(path);

    for(int y = 0; y < sizeY; y++) {
        for(int x = 0; x < sizeX; x++) {
            //TODO: use json? or xml?
            mapFile.get(tile);
            Game::AddTile(atoi(&tile), x * 16 * 3, y * 16 * 3, 16, 16); // TODO: don't like the magic number -- we need to know tile size and scale here
            mapFile.ignore();
        }
    }

    mapFile.close();
}
void Map::LoadMapXml(std::string path) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("../assets/stupid_map_2.xml");
    if (!result)
        return;
    int tileswide = doc.child("tilemap").attribute("tileswide").as_int();
    int tileshigh = doc.child("tilemap").attribute("tileshigh").as_int();
    int tilewidth = doc.child("tilemap").attribute("tilewidth").as_int();
    int tileheight = doc.child("tilemap").attribute("tileheight").as_int();
    pugi::xpath_node_set layer0 = doc.select_nodes("/tilemap/layer[@number='0']");
    
    for (pugi::xpath_node tile : layer0[0].node().children()) {
        pugi::xml_node node = tile.node();
        int x = node.attribute("x").as_int();
        int y = node.attribute("y").as_int();
        int index = node.attribute("index").as_int();
        int rot = node.attribute("rot").as_int();
        if (index < 0) index = 3; // TODO: decide what happens

        Game::AddTile(index, x * tilewidth *2, y * tileheight *2, tilewidth, tileheight, rot);
    }

}
