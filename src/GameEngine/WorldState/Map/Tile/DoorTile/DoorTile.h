#pragma once

#include "../EnemyContainerTile/EnemyContainerTile.h"
#include "Door/Door.h"

/*
===========================================
    DoorTile
===========================================
*/

class DoorTile : public EnemyContainerTile {
public:
    Door* door;
    static texturePair_t gateTexture;

public:
    DoorTile(const iPoint2& _tileCoord);
    ~DoorTile();

    virtual rayTileHitReturn_t  RayTileHit      (RayHitMarker& hitInfo)  const override;
    virtual bool                PlayerTileHit()                          const override;
};