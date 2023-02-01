/*
 * map_manager.h:
 *
 * Parses map.csv file and loads it into memory, with tiles and textures.
 *
 */

#pragma once

#include <string>

#include "GameData.h"
#include "MapFile.h"
#include "global.h"

class MapManager {
public:
    void Init(GameData* _game_data) {
        gameData = _game_data;
    }

    void Exit() const {
        for (Tile* tile : gameData->Map.tiles)
            delete tile;
    }

    void LoadMap(const std::string& file) const {
        MapFile mapFile(file);

        gameData->Map.width = mapFile.columns;
        gameData->Map.height = mapFile.rows;
        gameData->Map.numTiles = mapFile.numCells;

        // Grab door textures to feed into DoorTiles
        texturePairsPair_t door_textures = {gameData->Multimedia.get_wall_texture_pair(99), gameData->Multimedia.get_wall_texture_pair(101)};

        // Allocate enough memory and fill map with tiles corresponding to mapFile data
        gameData->Map.tiles.reserve(mapFile.numCells);
        for (int tileIndex = 0; tileIndex < mapFile.numCells; ++tileIndex) {
            auto tileInfo = mapFile.tiles[tileIndex];

            if (tileInfo.has_value() == false)
                gameData->Map.tiles.push_back(new EmptyTile());
            else {
                auto texture_info = tileInfo.value();
                auto [TextureType, TextureID] = tileInfo.value();
                switch (TextureType) {
                    case TEXTURE_WALLS:
                        if (TextureID == 99)
                            gameData->Map.tiles.push_back(new DoorTile(door_textures));
                        else {
                            auto wallTexturePair = gameData->Multimedia.get_wall_texture_pair(TextureID);
                            gameData->Map.tiles.push_back(new WallTile(wallTexturePair));
                        }
                        break;
                    case TEXTURE_OBJECTS:
                    case TEXTURE_GUARD:
                        Point2 tileCenterPt = GetTileCenterPt(tileIndex, mapFile.columns);
                        auto spriteTexture = gameData->Multimedia.get_texture_pair(texture_info);
                        SpriteTile *s = new SpriteTile(tileCenterPt, spriteTexture);
                        gameData->Map.tiles.push_back(s);
                        gameData->Map.sprites.push_back(s);
                        break;
                }
            }
        }
    }

    void UpdateSpritePerpLines() {
        for (SpriteTile* s : gameData->Map.sprites)
            s->CalculatePerpLine(gameData->Player.viewDir);
    }

private:
    Point2 GetTileCenterPt(const int& index, const int& map_width) const {
        int x = index % map_width;
        int y = index / map_width;
        return Point2(x + 0.5, y + 0.5);
    }
    
private:
    GameData* gameData;
};
