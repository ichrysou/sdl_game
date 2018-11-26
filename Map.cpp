#include "Map.h"
#include "game.h"
#include <fstream>

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
            Game::AddTile(atoi(&tile), x * 32, y * 32); // TODO: don't like the magic number -- we need to know tile size and scale here
            mapFile.ignore();
        }
    }

    mapFile.close();
}
