#pragma once

#include "Tile/Tile.h"
#include "Tile/DoorTile/DoorTile.h"
#include "Tile/SpriteTile/SpriteTile.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

typedef std::vector<std::vector<Tile*>> tileGrid2D_t;

/*
=========================================================
    Map
=========================================================
*/

class Map {
public:
    /* Tiles */
    tileGrid2D_t                     tiles;
    int                              width;
    int                              height;

    /* Doors */
    std::map<DoorTile*, DoorTile*>   activeDoors;
    bool                             anyDoorsAwaitingRendering;

public:
    Tile*                             GetTile               (const iPoint2& tileCoord)                     const;
    void                              SetTile               (const iPoint2& tileCoord, Tile* const tile);
    bool                              WithinMap             (const iPoint2& tileCoord)                     const;
    void                              AddActiveDoor         (DoorTile* const door);
    void                              RemoveActiveDoor      (DoorTile* const door);
    std::optional<DoorTile*>          GetLoneActiveDoor()                                                  const;
};