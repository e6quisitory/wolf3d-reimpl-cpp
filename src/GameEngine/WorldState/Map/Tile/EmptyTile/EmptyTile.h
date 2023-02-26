#pragma once

#include "../Tile.h"

/*
=========================================================
    Empty Tile
=========================================================
*/

class EmptyTile : public Tile {
public:
    EmptyTile();

    virtual rayTileHitVariant_o  RayTileHit       (RayHitMarker& hitInfo) const override;
    virtual bool                 PlayerTileHit()                          const override;
};