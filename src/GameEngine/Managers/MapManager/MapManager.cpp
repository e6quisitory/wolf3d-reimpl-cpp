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
    for (auto& mapColumn : worldState->map.tiles)
        for (auto& mapTile : mapColumn)
            delete mapTile;
}

void MapManager::LoadMap(const std::string file) const {
    MapFile mapFile(file);

    worldState->map.width  = mapFile.numColumns;
    worldState->map.height = mapFile.numRows;

    // Grab door textures to feed into DoorTiles
    texturePairsPair_t door_textures = {multimedia->GetWallTexturePair(99), multimedia->GetWallTexturePair(101)};

    // Allocate enough memory and fill map with tiles corresponding to mapFile data
    worldState->map.tiles.resize(mapFile.numColumns);
    for (auto& column : worldState->map.tiles)
        column.resize(mapFile.numRows);

    // Iterate through map csv file data and fill worldState->Map.tiles accordingly (by tile type)
    for (const auto& parsedColumn : mapFile.parsedTiles) {
        for (const auto& parsedTile : parsedColumn) {
            const auto& [parsedTileInfo_o, tileCoord] = parsedTile;
            if (parsedTileInfo_o.has_value()) {
                auto parsedTileInfo = parsedTileInfo_o.value();
                switch (parsedTileInfo.textureType) {
                    /* Wall tile */
                    case textureType_t::WALLS:
                        if (parsedTileInfo.textureID == 99)
                            worldState->map.SetTile(tileCoord, new DoorTile(tileCoord, door_textures));
                        else {
                            auto wallTexturePair = multimedia->GetWallTexturePair(parsedTileInfo.textureID);
                            worldState->map.SetTile(tileCoord, new WallTile(tileCoord, wallTexturePair));
                        }
                        break;

                    /* Sprite Tile */
                    case textureType_t::OBJECTS:
                    case textureType_t::GUARD:
                        texturePair_t  spriteTexture  = multimedia->GetTexturePair(parsedTileInfo.textureType, parsedTileInfo.textureID);
                        SpriteTile     *s             = new SpriteTile(tileCoord, spriteTexture);
                        worldState->map.SetTile(tileCoord, s);
                        worldState->map.sprites.push_back(s);
                        break;
                }
            } else {
                /* Empty tile */
                worldState->map.SetTile(tileCoord, new EmptyTile(tileCoord));
            }
        }
    }
}

void MapManager::UpdateSpritePerplines() const {
    static const double negNinetyDeg = -PI / 2;
    Vec2 playerViewDirPerp = worldState->player.viewDir.Rotate(negNinetyDeg);
    SpriteTile::perplinesDir = playerViewDirPerp;
}