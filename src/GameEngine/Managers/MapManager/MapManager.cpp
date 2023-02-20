#include "MapManager.h"

#include "MapFile/MapFile.h"
#include "../../State/WorldState/Map/Tile/EmptyTile/EmptyTile.h"
#include "../../State/WorldState/Map/Tile/WallTile/WallTile.h"
#include "../../State/WorldState/Map/Tile/DoorTile/DoorTile.h"
#include "../../State/WorldState/Map/Tile/SpriteTile/SpriteTile.h"

/*
================================
    Public Methods
================================
*/

void MapManager::Init(WorldState* const _worldState, Multimedia* const _multimedia) {
    worldState  = _worldState;
    multimedia = _multimedia;
}

void MapManager::Exit() const {
    for (Tile* const tile : worldState->map.tiles)
        delete tile;
}

void MapManager::LoadMap(const std::string file) const {
    MapFile mapFile(file);

    worldState->map.width  = mapFile.columns;
    worldState->map.height = mapFile.rows;
    worldState->map.numTiles  = mapFile.numCells;

    // Grab door textures to feed into DoorTiles
    texturePairsPair_t door_textures = {multimedia->GetWallTexturePair(99), multimedia->GetWallTexturePair(101)};

    // Allocate enough memory and fill map with tiles corresponding to mapFile data
    worldState->map.tiles.reserve(mapFile.numCells);

    // Iterate through map csv file data and fill worldState->Map.tiles accordingly (by tile type)
    for (const auto& parsedTile : mapFile.tiles) {
        if (parsedTile.has_value()) {
            switch (parsedTile->textureType) {

                /* Wall tile */
                case textureType_t::WALLS:
                    if (parsedTile->textureID == 99)
                        worldState->map.tiles.push_back(new DoorTile(parsedTile->tileCoord, door_textures));
                    else {
                        auto wallTexturePair = multimedia->GetWallTexturePair(parsedTile->textureID);
                        worldState->map.tiles.push_back(new WallTile(parsedTile->tileCoord, wallTexturePair));
                    }
                    break;

                /* Sprite Tile */
                case textureType_t::OBJECTS:
                case textureType_t::GUARD:
                    texturePair_t  spriteTexture  = multimedia->GetTexturePair(parsedTile->textureType, parsedTile->textureID);
                    SpriteTile     *s             = new SpriteTile(parsedTile->tileCoord, spriteTexture);
                    worldState->map.tiles.push_back(s);
                    worldState->map.sprites.push_back(s);
                    break;

            }
        } else {
            /* Empty tile */
            worldState->map.tiles.push_back(new EmptyTile(parsedTile->tileCoord));
        }
    }
}

void MapManager::UpdateSpritePerplines() const {
    static const double negNinetyDeg = -PI / 2;
    Vec2 playerViewDirPerp = worldState->player.viewDir.Rotate(negNinetyDeg);
    SpriteTile::perplinesDir = playerViewDirPerp;
}