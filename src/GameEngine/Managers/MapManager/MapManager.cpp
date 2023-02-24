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

    // Grab door gate texture and pass it to DoorTile class
    DoorTile::gateTexture = multimedia->GetWallTexturePair(99);

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
                    {
                        if (parsedTileInfo.textureID == 99)
                            worldState->map.SetTile(tileCoord, new DoorTile(tileCoord));
                        else {
                            auto wallTexturePair = multimedia->GetWallTexturePair(parsedTileInfo.textureID);
                            worldState->map.SetTile(tileCoord, new WallTile(tileCoord, wallTexturePair));
                        }
                        break;
                    }

                    /* Sprite Tile */
                    static const std::array<int, 21> noPassthroughList = {4, 5, 6, 8, 10, 11, 13, 14, 15, 16, 19, 20, 21, 25, 38, 39, 40, 42, 48, 49, 50};
                    case textureType_t::OBJECTS:
                    {
                        texturePair_t objectTexture = multimedia->GetTexturePair(textureType_t::OBJECTS, parsedTileInfo.textureID);
                        bool objectPassthrough = !(std::find(noPassthroughList.begin(), noPassthroughList.end(), parsedTileInfo.textureID) != noPassthroughList.end());
                        worldState->map.SetTile(tileCoord, new SpriteTile(tileCoord, objectTexture, objectPassthrough));
                        break;
                    }
                    case textureType_t::GUARD:
                    {
                        texturePair_t guardTexture = multimedia->GetTexturePair(textureType_t::GUARD, parsedTileInfo.textureID);
                        bool guardPassthrough = parsedTileInfo.textureID == 45;
                        worldState->map.SetTile(tileCoord, new SpriteTile(tileCoord, guardTexture, guardPassthrough));
                        break;
                    }
                }
            } else {
                /* Empty tile */
                worldState->map.SetTile(tileCoord, new EmptyTile(tileCoord));
            }
        }
    }
}