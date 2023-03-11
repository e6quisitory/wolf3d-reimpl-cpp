#pragma once

#include "../EnemyContainerTile/EnemyContainerTile.h"

/*
=========================================================
    Empty Tile
=========================================================
*/

class EmptyTile : public EnemyContainerTile {
public:
    EmptyTile(const iPoint2 _tileCoord);

    virtual rayTileHitReturn_t  RayTileHit       (RayHitMarker& hitInfo) const override;
    virtual bool                PlayerTileHit()                          const override;
};