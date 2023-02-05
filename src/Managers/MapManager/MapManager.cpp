#include <string>

#include "MapManager.h"
#include "../../MapFile.h"
#include "../../Utilities/Conventions.h"

/*
================================
Public Methods
================================
*/

void MapManager::Init(GameData* _gameData) {
    gameData = _gameData;
}

void MapManager::Exit() const {
    for (Tile* const tile : gameData->map.tiles)
        delete tile;
}

void MapManager::LoadMap(const std::string& file) const {
    MapFile mapFile(file);

    gameData->map.mapWidth    = mapFile.columns;
    gameData->map.mapHeight   = mapFile.rows;
    gameData->map.numTiles    = mapFile.numCells;

    // Grab door textures to feed into DoorTiles
    texturePairsPair_t door_textures = {gameData->multimedia.GetWallTexturePair(99),
                                        gameData->multimedia.GetWallTexturePair(101)};

    // Allocate enough memory and fill map with tiles corresponding to mapFile data
    gameData->map.tiles.reserve(mapFile.numCells);

    // Iterate through map csv file data and fill gameData->Map.tiles accordingly (by tile type)
    for (const auto& parsedTile : mapFile.tiles) {
        if (parsedTile.has_value()) {
            switch (parsedTile->textureType) {

                /* Wall tile */
                case textureType_t::WALLS:
                    if (parsedTile->textureID == 99)
                        gameData->map.tiles.push_back(new DoorTile(door_textures));
                    else {
                        auto wallTexturePair = gameData->multimedia.GetWallTexturePair(parsedTile->textureID);
                        gameData->map.tiles.push_back(new WallTile(wallTexturePair));
                    }
                    break;

                /* Sprite Tile */
                case textureType_t::OBJECTS:
                case textureType_t::GUARD:
                    Point2         tileCenterPt   = GetTileCenterPt(parsedTile->index, mapFile.columns);
                    texturePair_t  spriteTexture  = gameData->multimedia.GetTexturePair(parsedTile->textureType, parsedTile->textureID);
                    SpriteTile     *s             = new SpriteTile(tileCenterPt, spriteTexture);
                    gameData->map.tiles.push_back(s);
                    gameData->map.sprites.push_back(s);
                    break;

            }
        } else {

                /* Empty tile */
                gameData->map.tiles.push_back(new EmptyTile());

        }
    }
}

void MapManager::UpdateSpritePerpLines() {
    for (SpriteTile* const s : gameData->map.sprites)
        s->CalculatePerpLine(gameData->player.viewDir);
}

/*
================================
    Private Methods
================================
*/

Point2 MapManager::GetTileCenterPt(const int& index, const int& mapWidth) const {
    int x = index % mapWidth;
    int y = index / mapWidth;
    return Point2(x + 0.5, y + 0.5);
}