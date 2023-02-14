#include "MapManager.h"

#include "../../State/GameState/Map/Tile/EmptyTile/EmptyTile.h"
#include "../../State/GameState/Map/Tile/WallTile/WallTile.h"
#include "../../State/GameState/Map/Tile/DoorTile/DoorTile.h"
#include "../../State/GameState/Map/Tile/SpriteTile/SpriteTile.h"

/*
================================
    Public Methods
================================
*/

void MapManager::Init(GameState* const _gameState, Multimedia* const _multimedia) {
    gameState  = _gameState;
    multimedia = _multimedia;
}

void MapManager::Exit() const {
    for (Tile* const tile : gameState->map.tiles)
        delete tile;
}

void MapManager::LoadMap(const std::string file) const {
    MapFile mapFile(file);

    gameState->map.mapWidth    = mapFile.columns;
    gameState->map.mapHeight   = mapFile.rows;
    gameState->map.numTiles    = mapFile.numCells;

    // Grab door textures to feed into DoorTiles
    texturePairsPair_t door_textures = {multimedia->GetWallTexturePair(99),
                                        multimedia->GetWallTexturePair(101)};

    // Allocate enough memory and fill map with tiles corresponding to mapFile data
    gameState->map.tiles.reserve(mapFile.numCells);

    // Iterate through map csv file data and fill gameState->Map.tiles accordingly (by tile type)
    for (const auto& parsedTile : mapFile.tiles) {
        if (parsedTile.has_value()) {
            switch (parsedTile->textureType) {

                /* Wall tile */
                case textureType_t::WALLS:
                    if (parsedTile->textureID == 99)
                        gameState->map.tiles.push_back(new DoorTile(door_textures));
                    else {
                        auto wallTexturePair = multimedia->GetWallTexturePair(parsedTile->textureID);
                        gameState->map.tiles.push_back(new WallTile(wallTexturePair));
                    }
                    break;

                /* Sprite Tile */
                case textureType_t::OBJECTS:
                case textureType_t::GUARD:
                    Point2         tileCenterPt   = GetTileCenterPt(parsedTile->tileIndex, mapFile.columns);
                    texturePair_t  spriteTexture  = multimedia->GetTexturePair(parsedTile->textureType, parsedTile->textureID);
                    SpriteTile     *s             = new SpriteTile(tileCenterPt, spriteTexture);
                    gameState->map.tiles.push_back(s);
                    gameState->map.sprites.push_back(s);
                    break;

            }
        } else {

                /* Empty tile */
                gameState->map.tiles.push_back(new EmptyTile());

        }
    }
}

void MapManager::UpdateSpritePerpLines() {
    for (SpriteTile* const s : gameState->map.sprites)
        s->CalculatePerpLine(gameState->player.viewDir);
}

/*
================================
    Private Methods
================================
*/

Point2 MapManager::GetTileCenterPt(const int index, const int mapWidth) const {
    int x = index % mapWidth;
    int y = index / mapWidth;
    return Point2(x + 0.5, y + 0.5);
}