#include <string>

#include "MapManager.h"
#include "../MapFile.h"
#include "../Utilities/Conventions.h"

/*
================================
Public Methods
================================
*/

void MapManager::Init(GameData* _gameData) {
    gameData = _gameData;
}

void MapManager::Exit() const {
    for (Tile* tile : gameData->Map.tiles)
        delete tile;
}

void MapManager::LoadMap(const std::string& file) const {
    MapFile mapFile(file);

    gameData->Map.width    = mapFile.columns;
    gameData->Map.height   = mapFile.rows;
    gameData->Map.numTiles = mapFile.numCells;

    // Grab door textures to feed into DoorTiles
    texturePairsPair_t door_textures = {gameData->Multimedia.get_wall_texture_pair(99), gameData->Multimedia.get_wall_texture_pair(101)};

    // Allocate enough memory and fill map with tiles corresponding to mapFile data
    gameData->Map.tiles.reserve(mapFile.numCells);

    // Iterate through map csv file data and fill gameData->Map.tiles accordingly
    for (auto parsedTile : mapFile.tiles) {
        auto [tileIndex, tileTextureInfo] = parsedTile;

        if (tileTextureInfo.has_value()) {
            auto [textureType, textureID] = tileTextureInfo.value();

            switch (textureType) {
                case textureType_t::WALLS:
                    if (textureID == 99)
                        gameData->Map.tiles.push_back(new DoorTile(door_textures));
                    else {
                        auto wallTexturePair = gameData->Multimedia.get_wall_texture_pair(textureID);
                        gameData->Map.tiles.push_back(new WallTile(wallTexturePair));
                    }
                    break;
                case textureType_t::OBJECTS:
                case textureType_t::GUARD:
                    Point2         tileCenterPt   = GetTileCenterPt(tileIndex, mapFile.columns);
                    texturePair_t  spriteTexture  = gameData->Multimedia.get_texture_pair(tileTextureInfo.value());
                    SpriteTile     *s             = new SpriteTile(tileCenterPt, spriteTexture);
                    gameData->Map.tiles.push_back(s);
                    gameData->Map.sprites.push_back(s);
                    break;
            }

        } else
            gameData->Map.tiles.push_back(new EmptyTile());
    }
}

void MapManager::UpdateSpritePerpLines() {
    for (SpriteTile* s : gameData->Map.sprites)
        s->CalculatePerpLine(gameData->Player.viewDir);
}

/*
================================
    Private Methods
================================
*/

Point2 MapManager::GetTileCenterPt(const int& index, const int& map_width) const {
    int x = index % map_width;
    int y = index / map_width;
    return Point2(x + 0.5, y + 0.5);
}