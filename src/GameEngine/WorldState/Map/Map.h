#pragma once

#include "Tile/Tile.h"
#include "Tile/DoorTile/Door/Door.h"

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
    std::map<Door*, Door*>           activeDoors;

public:
    Tile*                             GetTile               (const iPoint2& tileCoord)                     const;
    void                              SetTile               (const iPoint2& tileCoord, Tile* const tile);
    bool                              WithinMap             (const iPoint2& tileCoord)                     const;
    void                              AddActiveDoor         (Door* const door);
    void                              RemoveActiveDoor      (Door* const door);
    std::optional<Door*>              GetLoneActiveDoor()                                                  const;
};