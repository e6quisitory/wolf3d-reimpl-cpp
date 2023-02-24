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
    DoorTile(const iPoint2& tileCoord);
    ~DoorTile();

    virtual textureSliceDistPair_o  RayTileHit      (RayHitMarker& hitInfo, const texturePair_o textureOverride) const override;
    virtual bool                    PlayerTileHit()                                                              const override;
};