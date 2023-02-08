#pragma once

#include <vector>
#include <map>

#include "../../../Utilities/Vec2D.h"
#include "../../../Tile.h"

class Map {
public:
    /* Tiles */
    std::vector<Tile*>               tiles;
    int                              mapWidth;
    int                              mapHeight;
    int                              numTiles;

    /* Doors */
    std::map<DoorTile*, DoorTile*>   activeDoors;
    bool                             anyDoorsAwaitingRendering;

    /* Sprites */
    std::vector<SpriteTile*>         sprites;

public:
    Tile*                             operator []           (const int& tileIndex)              const;
    Tile*                             operator ()           (const int& tileCoordX, const int& tileCoordY)    const;

    Tile*                             GetTile               (const iPoint2& tileCoord)            const;
    bool                              WithinMap             (const iPoint2& tileCoord)             const;
    void                              AddActiveDoor         (DoorTile* const door);
    void                              RemoveActiveDoor      (DoorTile* const door);
    std::optional<DoorTile*>          GetLoneActiveDoor()                                   const;
};