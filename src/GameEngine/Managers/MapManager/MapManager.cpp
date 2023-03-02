#include "MapManager.h"

#include "MapFile/MapFile.h"
#include "../../WorldState/Map/Tile/EmptyTile/EmptyTile.h"
#include "../../WorldState/Map/Tile/WallTile/WallTile.h"
#include "../../WorldState/Map/Tile/DoorTile/DoorTile.h"
#include "../../WorldState/Map/Tile/SpriteTile/ObjectTile/ObjectTile.h"
#include "../../WorldState/Map/Tile/SpriteTile/EnemyTile/EnemyTile.h"

/*
=========================================================
    Public methods
=========================================================
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
    DoorTile::gateTexture  = multimedia->GetWallTexturePair(99);

    // Set ObjectTile list of object textures that player cannot pass through
    const std::array<int, 21> objectNoPassthroughTextureIDs = {4, 5, 6, 8, 10, 11, 13, 14, 15, 16, 19, 20, 21, 25, 38, 39, 40, 42, 48, 49, 50};
    for (const int ID : objectNoPassthroughTextureIDs)
        ObjectTile::noPassthroughList.push_back(multimedia->GetTexture(textureType_t::OBJECTS, ID));

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
                            worldState->map.SetTile(tileCoord, new DoorTile());
                        else {
                            auto wallTexturePair = multimedia->GetWallTexturePair(parsedTileInfo.textureID);
                            worldState->map.SetTile(tileCoord, new WallTile(wallTexturePair));
                        }
                        break;
                    }

                    /* Object Tile */
                    case textureType_t::OBJECTS:
                    {
                        SDL_Texture* objectTexture = multimedia->GetTexture(textureType_t::OBJECTS, parsedTileInfo.textureID);
                        worldState->map.SetTile(tileCoord, new ObjectTile(tileCoord, objectTexture));
                        break;
                    }

                    /* Enemy Tile */
                    case textureType_t::GUARD:
                    {
                        SDL_Texture* guardTexture = multimedia->GetTexture(textureType_t::GUARD, parsedTileInfo.textureID);
                        worldState->map.SetTile(tileCoord, new EnemyTile(tileCoord, guardTexture));
                        break;
                    }
                }
            } else {
                /* Empty tile */
                worldState->map.SetTile(tileCoord, new EmptyTile());
            }
        }
    }
}