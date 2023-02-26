#pragma once

#include "../Tile.h"
#include "Door/Door.h"

/*
===========================================
    DoorTile
===========================================
*/

class DoorTile : public Tile {
public:
    Door* door;
    static texturePair_t gateTexture;

public:
    DoorTile();
    ~DoorTile();

    virtual rayTileHitVariant_o  RayTileHit      (RayHitMarker& hitInfo) const override;
    virtual bool                 PlayerTileHit()                         const override;
};